<h1 align="center">Game Object Sequences</h1>

This document describes the runtime sequence of each main object in Zomwar. Each object is handled by its own AK task and receives signals from the screen task (`scr_game_zomwar`), button callbacks, the periodic game tick timer, or other object tasks.

## I. Object Summary

| Object | Task ID | Handler | Main responsibility |
|---|---|---|---|
| Gunner | `ZW_GAME_GUNNER_ID` | `zw_game_gunner_handle()` | Controls the player position and gunner image state. |
| Bullet | `ZW_GAME_BULLET_ID` | `zw_game_bullet_handle()` | Shoots bullets, moves active bullets, and hides them when they exit the screen. |
| Zombie | `ZW_GAME_ZOMBIE_ID` | `zw_game_zombie_handle()` | Spawns zombies, moves them (zigzag, rising from tombstones), checks collision with bullets, and updates score. |
| Bang | `ZW_GAME_BANG_ID` | `zw_game_bang_handle()` | Plays explosion animation after a zombie is hit. |
| Border | `ZW_GAME_BORDER_ID` | `zw_game_border_handle()` | Checks wave level-up and game-over conditions. |
| Car | `ZW_GAME_CAR_ID` | `zw_game_car_handle()` | Lawnmower-style rescue cars that activate when a zombie reaches the left edge or touches a parked car. |
| Tombstone | `ZW_GAME_TOMBSTONE_ID` | `zw_game_tombstone_handle()` | Spawns extra zombies that rise out of active tombstones. |

The screen task posts `ZW_GAME_TIME_TICK` every `ZW_GAME_TIME_TICK_INTERVAL` (100 ms). On each tick the screen task fans out signals to every object task in a fixed order.

## II. Gunner Object Sequence

Gunner owns the player position (`gunner`).

**Setup.** `ZW_GAME_GUNNER_SETUP` parks the gunner at `(GUNNER_AXIS_X, GUNNER_AXIS_Y)` with `visible = WHITE`, `action_image = 1`.

**Input.** Button callbacks only update `gunner_dir` inside the screen task; they do not post to the Gunner task directly.

**Per-tick.** Each `ZW_GAME_TIME_TICK` the screen translates the latched `gunner_dir` into `ZW_GAME_GUNNER_UP` / `ZW_GAME_GUNNER_DOWN` (when non-zero) and always posts `ZW_GAME_GUNNER_UPDATE`.

- `UP` / `DOWN`: moves `gunner.y` by `GUNNER_STEP_AXIS_Y`, clamped to `GUNNER_AXIS_Y_MIN..GUNNER_AXIS_Y_MAX`.
- `UPDATE`: clears the recoil frame by resetting `gunner.action_image` from `2` back to `1` (the recoil frame is raised by the Bullet task on `ZW_GAME_BULLET_SHOOT`).

**Reset.** `ZW_GAME_GUNNER_RESET` re-parks the gunner and sets `visible = BLACK`.

```mermaid
%%{init: {'theme':'base','themeVariables':{'fontSize':'18px','primaryColor':'#1565c0','primaryTextColor':'#ffffff','primaryBorderColor':'#0d47a1','lineColor':'#90a4ae','signalColor':'#ffc107','signalTextColor':'#ffc107','actorBkg':'#1565c0','actorBorder':'#0d47a1','actorTextColor':'#ffffff','actorLineColor':'#90caf9','noteBkgColor':'#fff59d','noteTextColor':'#000000','noteBorderColor':'#f57f17','activationBkgColor':'#66bb6a','activationBorderColor':'#2e7d32','sequenceNumberColor':'#ffffff','loopTextColor':'#ffc107','labelBoxBkgColor':'#37474f','labelBoxBorderColor':'#90a4ae','labelTextColor':'#ffffff'},'sequence':{'actorMargin':120,'messageFontSize':17,'noteFontSize':15,'actorFontSize':17,'boxMargin':15,'boxTextMargin':8,'noteMargin':12,'useMaxWidth':false}}}%%
sequenceDiagram
    autonumber
    actor Btn as Button
    participant Scr as Screen task
    participant Gun as Gunner task

    Note over Scr: SCREEN_ENTRY
    Scr-)Gun: ZW_GAME_GUNNER_SETUP
    activate Gun
    Note right of Gun: init pose at (X, Y)<br/>visible = WHITE, action_image = 1
    deactivate Gun
    Note over Scr: arm 100 ms periodic tick

    Note over Btn,Scr: Button events fire asynchronously (between ticks)
    Btn-)Scr: AC_DISPLAY_BUTTON_UP_PRESSED
    activate Scr
    Note right of Scr: gunner_dir = UP
    deactivate Scr
    Btn-)Scr: AC_DISPLAY_BUTTON_DOWN_PRESSED
    activate Scr
    Note right of Scr: gunner_dir = DOWN
    deactivate Scr
    Btn-)Scr: AC_DISPLAY_BUTTON_*_RELEASED
    activate Scr
    Note right of Scr: if dir matches released key<br/>then dir = NONE
    deactivate Scr

    loop Each ZW_GAME_TIME_TICK
        alt gunner_dir == UP
            Scr-)Gun: ZW_GAME_GUNNER_UP
            activate Gun
            Note right of Gun: gunner.y -= GUNNER_STEP_AXIS_Y<br/>clamp >= GUNNER_AXIS_Y_MIN
            deactivate Gun
        else gunner_dir == DOWN
            Scr-)Gun: ZW_GAME_GUNNER_DOWN
            activate Gun
            Note right of Gun: gunner.y += GUNNER_STEP_AXIS_Y<br/>clamp <= GUNNER_AXIS_Y_MAX
            deactivate Gun
        end
        Scr-)Gun: ZW_GAME_GUNNER_UPDATE
        activate Gun
        Note right of Gun: clear recoil frame (2 -> 1)
        deactivate Gun
    end

    Note over Scr: on ZW_GAME_RESET
    Scr-)Gun: ZW_GAME_GUNNER_RESET
    activate Gun
    Note right of Gun: re-park, visible = BLACK
    deactivate Gun
```

<p align="center"><strong><em>Figure 1:</em></strong> Gunner sequence logic</p>

## III. Bullet Object Sequence

Bullet owns the `bullet[BULLET_NUMBER]` array and handles shooting from the MODE button (only while `zw_game_state == GAME_PLAY`).

**Setup.** `ZW_GAME_BULLET_SETUP` clears every slot (`visible = BLACK`, `x = y = 0`).

**Input.** On `AC_DISPLAY_BUTTON_MODE_PRESSED` the screen task posts `ZW_GAME_BULLET_SHOOT` (only while playing). The handler picks the first free slot in `bullet[]`, spawns it at `(gunner.x + BULLET_SPAWN_OFFSET_X, gunner.y - BULLET_SPAWN_OFFSET_Y)`, sets `gunner.action_image = 2` to show the recoil frame, and plays `BUZZER_SOUND_CLICK`.

**Per-tick.** Each `ZW_GAME_TIME_TICK` the screen task posts `ZW_GAME_BULLET_RUN`: every visible bullet steps right by `BULLET_STEP_AXIS_X`. When `x >= BULLET_MAX_AXIS_X` the bullet is hidden and its `x` is cleared.

**Reset.** `ZW_GAME_BULLET_RESET` clears every slot (same as setup).

```mermaid
%%{init: {'theme':'base','themeVariables':{'fontSize':'18px','primaryColor':'#1565c0','primaryTextColor':'#ffffff','primaryBorderColor':'#0d47a1','lineColor':'#90a4ae','signalColor':'#ffc107','signalTextColor':'#ffc107','actorBkg':'#1565c0','actorBorder':'#0d47a1','actorTextColor':'#ffffff','actorLineColor':'#90caf9','noteBkgColor':'#fff59d','noteTextColor':'#000000','noteBorderColor':'#f57f17','activationBkgColor':'#66bb6a','activationBorderColor':'#2e7d32','sequenceNumberColor':'#ffffff','loopTextColor':'#ffc107','labelBoxBkgColor':'#37474f','labelBoxBorderColor':'#90a4ae','labelTextColor':'#ffffff'},'sequence':{'actorMargin':120,'messageFontSize':17,'noteFontSize':15,'actorFontSize':17,'boxMargin':15,'boxTextMargin':8,'noteMargin':12,'useMaxWidth':false}}}%%
sequenceDiagram
    autonumber
    actor Btn as Button
    participant Scr as Screen task
    participant Bul as Bullet task
    participant Bz as Buzzer

    Note over Scr: SCREEN_ENTRY
    Scr-)Bul: ZW_GAME_BULLET_SETUP
    activate Bul
    Note right of Bul: bullet[*]: visible=BLACK, x=y=0
    deactivate Bul
    Note over Scr: arm 100 ms periodic tick

    Note over Btn,Scr: MODE button fires asynchronously
    Btn-)Scr: AC_DISPLAY_BUTTON_MODE_PRESSED
    activate Scr
    opt zw_game_state == GAME_PLAY
        Scr-)Bul: ZW_GAME_BULLET_SHOOT
    end
    deactivate Scr
    activate Bul
    Note right of Bul: pick first slot with visible!=WHITE<br/>visible=WHITE, x=gunner.x+BULLET_SPAWN_OFFSET_X, y=gunner.y-BULLET_SPAWN_OFFSET_Y<br/>gunner.action_image = 2 (recoil frame, shared global owned by Gunner)
    Bul->>+Bz: BUZZER_PlaySound(CLICK)
    Bz-->>-Bul: 
    deactivate Bul

    loop Each ZW_GAME_TIME_TICK
        Scr-)Bul: ZW_GAME_BULLET_RUN
        activate Bul
        loop for each visible bullet
            Note right of Bul: x += BULLET_STEP_AXIS_X
            alt x >= BULLET_MAX_AXIS_X
                Note right of Bul: visible=BLACK, x=0
            end
        end
        deactivate Bul
    end

    Note over Scr: on ZW_GAME_RESET
    Scr-)Bul: ZW_GAME_BULLET_RESET
    activate Bul
    Note right of Bul: bullet[*]: visible=BLACK, x=y=0
    deactivate Bul
```

<p align="center"><strong><em>Figure 2:</em></strong> Bullet sequence logic</p>

## IV. Zombie Object Sequence

Zombie owns the horde state: the `zombie[ZOMBIE_NUMBER]` array and `zw_game_zombie_speed`.

**Setup.** `ZW_GAME_ZOMBIE_SETUP` reads `zw_game_zombie_speed` from `settingsetup.zombie_speed`, hides every slot in `zombie[]`, then spawns the first `ZOMBIE_INIT_NUMBER` zombies at random `(x, y)` inside the right margin (`x` in `ZOMBIE_SPAWN_X_MIN..MAX`, `y` in `ZOMBIE_Y_MIN..MAX`).

**Per-tick.** Each `ZW_GAME_TIME_TICK` the screen task posts `ZW_GAME_ZOMBIE_RUN` followed by `ZW_GAME_ZOMBIE_DETONATOR`.

- `RUN`: each visible zombie either rises one pixel up (when it was spawned from a tombstone via `zw_game_zombie_spawn_from_tombstone()` and `rise_ticks > 0`; when `rise_ticks` reaches zero, `rising` is cleared and `zigzag_timer` is re-rolled), or steps left by `zw_game_zombie_speed` (clamped at `-ZOMBIE_MIN_LEFT_OFFSET`), applies a vertical zigzag (`dy` re-rolled to `-1..+1` every `zigzag_timer` ticks, clamped to `ZOMBIE_Y_MIN..ZOMBIE_Y_MAX` and reset to `0` on clamp), and cycles `action_image` through frames `1->2->3`. After moving, the task tops the alive count back up to `ZOMBIE_INIT_NUMBER` by re-spawning hidden slots.
- `DETONATOR`: for every visible non-rising zombie it walks `bullet[]`, calls `zw_game_zombie_check_hit()`, and on a hit hides the bullet (`visible = BLACK`, `x = 0`), calls `zw_game_bang_spawn()`, adds `10` to `zw_game_score`, plays `BUZZER_SOUND_BANG`, and hides the zombie.

**Cross-task.** `ZW_GAME_ZOMBIE_WAVE_SPAWN` (posted from the Border task on level-up) increments `zw_game_zombie_speed` up to `ZOMBIE_SPEED_MAX` and respawns up to `ZOMBIE_WAVE_SPAWN` hidden slots.

**Reset.** `ZW_GAME_ZOMBIE_RESET` hides every slot.

```mermaid
%%{init: {'theme':'base','themeVariables':{'fontSize':'18px','primaryColor':'#1565c0','primaryTextColor':'#ffffff','primaryBorderColor':'#0d47a1','lineColor':'#90a4ae','signalColor':'#ffc107','signalTextColor':'#ffc107','actorBkg':'#1565c0','actorBorder':'#0d47a1','actorTextColor':'#ffffff','actorLineColor':'#90caf9','noteBkgColor':'#fff59d','noteTextColor':'#000000','noteBorderColor':'#f57f17','activationBkgColor':'#66bb6a','activationBorderColor':'#2e7d32','sequenceNumberColor':'#ffffff','loopTextColor':'#ffc107','labelBoxBkgColor':'#37474f','labelBoxBorderColor':'#90a4ae','labelTextColor':'#ffffff'},'sequence':{'actorMargin':120,'messageFontSize':17,'noteFontSize':15,'actorFontSize':17,'boxMargin':15,'boxTextMargin':8,'noteMargin':12,'useMaxWidth':false}}}%%
sequenceDiagram
    autonumber
    participant Scr as Screen task
    participant Bdr as Border task
    participant Tmb as Tombstone task
    participant Zmb as Zombie task
    participant Bz as Buzzer

    Note over Scr: SCREEN_ENTRY
    Scr-)Zmb: ZW_GAME_ZOMBIE_SETUP
    activate Zmb
    Note right of Zmb: speed = settingsetup.zombie_speed<br/>hide all slots<br/>spawn ZOMBIE_INIT_NUMBER zombies at random (x in ZOMBIE_SPAWN_X_MIN..MAX, y in ZOMBIE_Y_MIN..MAX)
    deactivate Zmb
    Note over Scr: arm 100 ms periodic tick

    loop Each ZW_GAME_TIME_TICK
        Scr-)Zmb: ZW_GAME_ZOMBIE_RUN
        activate Zmb
        loop for each visible zombie
            alt rising
                Note right of Zmb: y--, rise_ticks--<br/>if ticks reach 0 then rising=false and re-roll zigzag_timer<br/>cycle action_image 1->2->3
            else moving left
                Note right of Zmb: x -= speed (clamp at -ZOMBIE_MIN_LEFT_OFFSET)<br/>y += dy, clamp to Y_MIN..Y_MAX, dy=0 on clamp<br/>if zigzag_timer hits 0, re-roll dy in -1..+1<br/>cycle action_image 1->2->3
            end
        end
        opt alive less than ZOMBIE_INIT_NUMBER
            Note right of Zmb: re-spawn hidden slots until count restored
        end
        deactivate Zmb

        Scr-)Zmb: ZW_GAME_ZOMBIE_DETONATOR
        activate Zmb
        loop for each visible non-rising zombie z, for each visible bullet b
            opt hit detected
                Note right of Zmb: hide bullet (visible=BLACK, x=0)<br/>zw_game_bang_spawn(z.x, z.y)<br/>zw_game_score += 10<br/>hide zombie (visible=BLACK)
                Zmb->>+Bz: BUZZER_PlaySound(BANG)
                Bz-->>-Zmb: 
            end
        end
        deactivate Zmb
    end

    Note over Tmb: during Tombstone's ZW_GAME_TOMBSTONE_SPAWN tick
    Tmb->>+Zmb: zw_game_zombie_spawn_from_tombstone(i, x, y)
    Note right of Zmb: target slot becomes visible=WHITE, rising=true<br/>rise_ticks=ZOMBIE_RISE_TICKS, action_image=1
    Zmb-->>-Tmb: 

    Note over Bdr: during Border's ZW_GAME_BORDER_LEVEL_UP
    Bdr-)Zmb: ZW_GAME_ZOMBIE_WAVE_SPAWN
    activate Zmb
    Note right of Zmb: speed++ (cap ZOMBIE_SPEED_MAX)<br/>re-spawn up to ZOMBIE_WAVE_SPAWN hidden slots
    deactivate Zmb

    Note over Scr: on ZW_GAME_RESET
    Scr-)Zmb: ZW_GAME_ZOMBIE_RESET
    activate Zmb
    Note right of Zmb: hide all slots
    deactivate Zmb
```

<p align="center"><strong><em>Figure 3:</em></strong> Zombie sequence logic</p>

## V. Car Object Sequence

Car owns the lawnmower-style rescue cars (`car[CAR_LANE_NUMBER]`, one slot per lane).

**Setup.** `ZW_GAME_CAR_SETUP` parks each car at `(CAR_AXIS_X, lane_y[i])`, sets `visible` from the i-th bit of `settingsetup.num_car`, and clears `running`.

**Per-tick.** Each `ZW_GAME_TIME_TICK` the screen task posts `ZW_GAME_CAR_RUN` then `ZW_GAME_CAR_HIT`.

- `RUN`: first walks `zombie[]`: a visible zombie that reaches the left edge (`x <= -ZOMBIE_MIN_LEFT_OFFSET`) wakes the nearest non-running car within `CAR_HIT_RANGE_Y` via `zw_game_car_find_nearest()`, that car starts running, `zw_game_bang_spawn()` plays, `zw_game_score += 10`, `BUZZER_SOUND_BANG` is played, and the zombie is hidden; a zombie still on screen whose nearest car overlaps it (`zw_game_car_check_hit`) wakes that car too (kill deferred to `HIT`). After the activation pass, every visible running car slides right by `CAR_SPEED`, cycles `action_image` through `1->2->3`, and despawns (`visible = false`, `running = false`) once `x > LCD_WIDTH`.
- `HIT`: every visible running car walks `zombie[]` and for each overlap calls `zw_game_bang_spawn()`, adds `10` to `zw_game_score`, plays `BUZZER_SOUND_BANG`, and hides the zombie.

**Reset.** `ZW_GAME_CAR_RESET` re-parks every lane and clears both `visible` and `running`.

```mermaid
%%{init: {'theme':'base','themeVariables':{'fontSize':'18px','primaryColor':'#1565c0','primaryTextColor':'#ffffff','primaryBorderColor':'#0d47a1','lineColor':'#90a4ae','signalColor':'#ffc107','signalTextColor':'#ffc107','actorBkg':'#1565c0','actorBorder':'#0d47a1','actorTextColor':'#ffffff','actorLineColor':'#90caf9','noteBkgColor':'#fff59d','noteTextColor':'#000000','noteBorderColor':'#f57f17','activationBkgColor':'#66bb6a','activationBorderColor':'#2e7d32','sequenceNumberColor':'#ffffff','loopTextColor':'#ffc107','labelBoxBkgColor':'#37474f','labelBoxBorderColor':'#90a4ae','labelTextColor':'#ffffff'},'sequence':{'actorMargin':120,'messageFontSize':17,'noteFontSize':15,'actorFontSize':17,'boxMargin':15,'boxTextMargin':8,'noteMargin':12,'useMaxWidth':false}}}%%
sequenceDiagram
    autonumber
    participant Scr as Screen task
    participant Bdr as Border task
    participant Car as Car task
    participant Bz as Buzzer

    Note over Scr: SCREEN_ENTRY
    Scr-)Car: ZW_GAME_CAR_SETUP
    activate Car
    Note right of Car: for each lane i: re-park at (CAR_AXIS_X, lane_y[i])<br/>running=false, action_image=1<br/>visible = bit i of settingsetup.num_car
    deactivate Car
    Note over Scr: arm 100 ms periodic tick

    loop Each ZW_GAME_TIME_TICK
        Scr-)Car: ZW_GAME_CAR_RUN
        activate Car
        Note right of Car: Phase 1 (activation pass)
        loop for each visible zombie z
            alt z reached left edge (z.x <= -ZOMBIE_MIN_LEFT_OFFSET)
                Note right of Car: m = zw_game_car_find_nearest(z.y)
                opt m >= 0
                    Note right of Car: car[m].running = true<br/>zw_game_bang_spawn(z.x, z.y)<br/>zw_game_score += 10<br/>hide z (visible=BLACK)
                    Car->>+Bz: BUZZER_PlaySound(BANG)
                    Bz-->>-Car: 
                end
            else z still on screen
                Note right of Car: m = zw_game_car_find_nearest(z.y)
                opt car found and overlaps z
                    Note right of Car: car[m].running = true<br/>kill deferred to CAR_HIT
                end
            end
        end
        Note right of Car: Phase 2 (advance running cars)
        loop for each visible running car c
            Note right of Car: c.x += CAR_SPEED<br/>cycle action_image 1->2->3
            opt c reaches right edge (c.x > LCD_WIDTH)
                Note right of Car: visible=false, running=false
            end
        end
        deactivate Car

        Scr-)Car: ZW_GAME_CAR_HIT
        activate Car
        loop for each visible running car c, for each visible zombie z
            opt hit detected
                Note right of Car: zw_game_bang_spawn(z.x, z.y)<br/>zw_game_score += 10<br/>hide z (visible=BLACK)
                Car->>+Bz: BUZZER_PlaySound(BANG)
                Bz-->>-Car: 
            end
        end
        deactivate Car
    end

    Note over Bdr: during Border's ZW_GAME_BORDER_CHECK_GAME_OVER
    Bdr->>+Car: zw_game_car_find_nearest(zy)
    Note right of Car: walk car array, return index of<br/>nearest non-running visible car within CAR_HIT_RANGE_Y, else -1
    Car-->>-Bdr: idx

    Note over Scr: on ZW_GAME_RESET
    Scr-)Car: ZW_GAME_CAR_RESET
    activate Car
    Note right of Car: for each lane: re-park, visible=false, running=false
    deactivate Car
```

<p align="center"><strong><em>Figure 4:</em></strong> Car sequence logic</p>


## VI. Tombstone Object Sequence

Tombstone owns the `tombstone[TOMBSTONE_NUMBER]` array, 2 tombstones per lane (group 1 at `x = TOMBSTONE_LANE_1_X_MIN..MAX`, group 2 at `x = TOMBSTONE_LANE_2_X_MIN..MAX`).

**Setup.** `ZW_GAME_TOMBSTONE_SETUP` arms `tombstone_spawn_timer = TOMBSTONE_SPAWN_INTERVAL` and, for each lane, sets `active` from the i-th bit of `settingsetup.tombstone_lane_1` (group 1) and `settingsetup.tombstone_lane_2` (group 2).

**Per-tick.** Each `ZW_GAME_TIME_TICK` the screen task posts `ZW_GAME_TOMBSTONE_SPAWN`.

- While `tombstone_spawn_timer > 0` the task just decrements it and returns.
- Once it reaches `0` the timer is rearmed and a random index `tidx = rand() % TOMBSTONE_NUMBER` is rolled. If `tombstone[tidx].active` is false the tick is skipped; otherwise it walks `zombie[]` to find the first hidden slot and calls `zw_game_zombie_spawn_from_tombstone(i, tombstone[tidx].x, lane_y[tombstone[tidx].lane] + TOMBSTONE_SIZE_BITMAP_Y)`, which starts a zombie rising out of the tombstone over `ZOMBIE_RISE_TICKS` frames.

**Reset.** `ZW_GAME_TOMBSTONE_RESET` clears the timer and zeros every slot (`x = 0`, `lane = 0`, `active = false`).

```mermaid
%%{init: {'theme':'base','themeVariables':{'fontSize':'18px','primaryColor':'#1565c0','primaryTextColor':'#ffffff','primaryBorderColor':'#0d47a1','lineColor':'#90a4ae','signalColor':'#ffc107','signalTextColor':'#ffc107','actorBkg':'#1565c0','actorBorder':'#0d47a1','actorTextColor':'#ffffff','actorLineColor':'#90caf9','noteBkgColor':'#fff59d','noteTextColor':'#000000','noteBorderColor':'#f57f17','activationBkgColor':'#66bb6a','activationBorderColor':'#2e7d32','sequenceNumberColor':'#ffffff','loopTextColor':'#ffc107','labelBoxBkgColor':'#37474f','labelBoxBorderColor':'#90a4ae','labelTextColor':'#ffffff'},'sequence':{'actorMargin':120,'messageFontSize':17,'noteFontSize':15,'actorFontSize':17,'boxMargin':15,'boxTextMargin':8,'noteMargin':12,'useMaxWidth':false}}}%%
sequenceDiagram
    autonumber
    participant Scr as Screen task
    participant Tmb as Tombstone task
    participant Zmb as Zombie task

    Note over Scr: SCREEN_ENTRY
    Scr-)Tmb: ZW_GAME_TOMBSTONE_SETUP
    activate Tmb
    Note right of Tmb: tombstone_spawn_timer = TOMBSTONE_SPAWN_INTERVAL<br/>for each lane l (0..CAR_LANE_NUMBER-1):<br/>  slot l (group 1): x=rand(TOMBSTONE_LANE_1_X_MIN..MAX), lane=l, active=bit l of tombstone_lane_1<br/>  slot l+CAR_LANE_NUMBER (group 2): x=rand(TOMBSTONE_LANE_2_X_MIN..MAX), lane=l, active=bit l of tombstone_lane_2
    deactivate Tmb
    Note over Scr: arm 100 ms periodic tick

    loop Each ZW_GAME_TIME_TICK
        Scr-)Tmb: ZW_GAME_TOMBSTONE_SPAWN
        activate Tmb
        alt tombstone_spawn_timer > 0
            Note right of Tmb: tombstone_spawn_timer--<br/>return early
        else timer reached 0
            Note right of Tmb: tombstone_spawn_timer = TOMBSTONE_SPAWN_INTERVAL<br/>tidx = rand() mod TOMBSTONE_NUMBER
            alt tombstone[tidx].active == false
                Note right of Tmb: return early
            else active
                loop i = 0 .. ZOMBIE_NUMBER-1 (until spawned)
                    alt zombie[i].visible != WHITE (hidden slot found)
                        Note right of Tmb: x = tombstone[tidx].x<br/>y = lane_y[tombstone[tidx].lane] + TOMBSTONE_SIZE_BITMAP_Y
                        Tmb->>+Zmb: zw_game_zombie_spawn_from_tombstone(i, x, y)
                        Note right of Zmb: slot i: visible=WHITE, rising=true<br/>rise_ticks=ZOMBIE_RISE_TICKS, action_image=1
                        Zmb-->>-Tmb: 
                        Note right of Tmb: break loop
                    end
                end
            end
        end
        deactivate Tmb
    end

    Note over Scr: on ZW_GAME_RESET
    Scr-)Tmb: ZW_GAME_TOMBSTONE_RESET
    activate Tmb
    Note right of Tmb: tombstone_spawn_timer = 0<br/>for each slot: x=0, lane=0, active=false
    deactivate Tmb
```

<p align="center"><strong><em>Figure 5:</em></strong> Tombstone sequence logic</p>

## VII. Bang Object Sequence

Bang owns the `bang[BANG_NUMBER]` array of short-lived explosion sprites that play whenever a zombie is killed. It exposes no spawn signal. Zombie (on a bullet hit, `zw_game_zombie.cpp`) and Car (on car activation at the screen edge and on a running car overlapping a zombie, `zw_game_car.cpp`) call `zw_game_bang_spawn(x, y)` directly.

**Setup.** `ZW_GAME_BANG_SETUP` calls `zw_game_bang_reset_all()`, clearing every slot to `visible = BLACK`, `action_image = 1`.

**Per-tick.** Each `ZW_GAME_TIME_TICK` the screen task posts `ZW_GAME_BANG_UPDATE`: for every visible slot, if `action_image >= 3` the slot is retired (`action_image = 1`, `visible = BLACK`); otherwise `action_image` is incremented. Each explosion plays frames `1 -> 2 -> 3` over three ticks (~300 ms at the 100 ms tick interval) before its slot becomes free for the next hit.

**Cross-task.** `zw_game_bang_spawn(x, y)` walks `bang[]`, picks the first slot whose `visible != WHITE`, and writes `visible = WHITE`, `x = max(x + BANG_SPAWN_OFFSET_X, 0)`, `y = max(y - BANG_SPAWN_OFFSET_Y, 0)`, `action_image = 1`. The small offset centers the explosion bitmap over the zombie hit point.

**Reset.** `ZW_GAME_BANG_RESET` runs the same `zw_game_bang_reset_all()` as setup.

```mermaid
%%{init: {'theme':'base','themeVariables':{'fontSize':'18px','primaryColor':'#1565c0','primaryTextColor':'#ffffff','primaryBorderColor':'#0d47a1','lineColor':'#90a4ae','signalColor':'#ffc107','signalTextColor':'#ffc107','actorBkg':'#1565c0','actorBorder':'#0d47a1','actorTextColor':'#ffffff','actorLineColor':'#90caf9','noteBkgColor':'#fff59d','noteTextColor':'#000000','noteBorderColor':'#f57f17','activationBkgColor':'#66bb6a','activationBorderColor':'#2e7d32','sequenceNumberColor':'#ffffff','loopTextColor':'#ffc107','labelBoxBkgColor':'#37474f','labelBoxBorderColor':'#90a4ae','labelTextColor':'#ffffff'},'sequence':{'actorMargin':120,'messageFontSize':17,'noteFontSize':15,'actorFontSize':17,'boxMargin':15,'boxTextMargin':8,'noteMargin':12,'useMaxWidth':false}}}%%
sequenceDiagram
    autonumber
    participant Scr as Screen task
    participant Zmb as Zombie task
    participant Car as Car task
    participant Bng as Bang task

    Note over Scr: SCREEN_ENTRY
    Scr-)Bng: ZW_GAME_BANG_SETUP
    activate Bng
    Note right of Bng: zw_game_bang_reset_all()<br/>for each slot: visible=BLACK, action_image=1
    deactivate Bng
    Note over Scr: arm 100 ms periodic tick

    loop Each ZW_GAME_TIME_TICK
        opt during Zombie's ZW_GAME_ZOMBIE_DETONATOR, on bullet hit
            Zmb->>+Bng: zw_game_bang_spawn(x, y)
            Note right of Bng: pick first slot with visible != WHITE<br/>visible=WHITE, x=max(x+BANG_SPAWN_OFFSET_X, 0), y=max(y-BANG_SPAWN_OFFSET_Y, 0)<br/>action_image=1
            Bng-->>-Zmb: 
        end

        opt during Car's ZW_GAME_CAR_RUN / ZW_GAME_CAR_HIT, on hit
            Car->>+Bng: zw_game_bang_spawn(x, y)
            Note right of Bng: same as above (first free slot)
            Bng-->>-Car: 
        end

        Scr-)Bng: ZW_GAME_BANG_UPDATE
        activate Bng
        loop for each slot i
            opt bang[i].visible == WHITE
                alt bang[i].action_image >= 3
                    Note right of Bng: action_image=1, visible=BLACK<br/>(retire slot, free for next hit)
                else action_image < 3
                    Note right of Bng: action_image++
                end
            end
        end
        deactivate Bng
    end

    Note over Scr: on ZW_GAME_RESET
    Scr-)Bng: ZW_GAME_BANG_RESET
    activate Bng
    Note right of Bng: zw_game_bang_reset_all()<br/>for each slot: visible=BLACK, action_image=1
    deactivate Bng
```

<p align="center"><strong><em>Figure 6:</em></strong> Bang sequence logic</p>

## VIII. Border Object Sequence

Border owns the game's progression state (`zw_game_score`, `wave_last_score`, `wave_level`, and the wave-warning latch `wave_warning_active`, `wave_warning_timer`) and is the sole arbiter of game over.

**Setup.** `ZW_GAME_BORDER_SETUP` calls `zw_game_border_clear()`, zeroing the score, last-wave score, wave level, warning timer, and warning flag.

**Per-tick.** Each `ZW_GAME_TIME_TICK` the screen task posts three Border signals in order: `ZW_GAME_BORDER_CHECK_GAME_OVER`, `ZW_GAME_BORDER_CHECK_WAVE`, then `ZW_GAME_BORDER_LEVEL_UP`.

- `ZW_GAME_BORDER_CHECK_GAME_OVER`: walks `zombie[]` and, for every visible zombie that has reached the left edge (`x <= -ZOMBIE_MIN_LEFT_OFFSET`), asks the Car task via `zw_game_car_find_nearest(zombie[i].y)` whether any rescue car is in range; if none is found for any such zombie, Border posts `ZW_GAME_RESET` to the screen task (`AC_TASK_DISPLAY_ID`) and stops scanning.
- `ZW_GAME_BORDER_CHECK_WAVE`: arms a new wave when the player crosses the next score threshold: if no warning is currently active and `zw_game_score >= wave_last_score + BORDER_WAVE_SCORE_INTERVAL` (200), it sets `wave_warning_active = true` and loads `wave_warning_timer = BORDER_WARNING_BLINK_DURATION` (30 ticks, about 3 s). The screen task uses that timer to blink the warning bitmap at `BORDER_WARNING_BLINK_RATE` and to render `wave_level` (`scr_game_zomwar.cpp`).
- `ZW_GAME_BORDER_LEVEL_UP`: runs only while the warning is active: it counts down `wave_warning_timer` by one each tick and, once the timer hits zero, clears the warning, advances `wave_last_score` by `BORDER_WAVE_SCORE_INTERVAL`, increments `wave_level`, and posts `ZW_GAME_ZOMBIE_WAVE_SPAWN` to the Zombie task so the next wave spawns and the zombie speed steps up.

**Reset.** `ZW_GAME_BORDER_RESET` runs the same `zw_game_border_clear()` as setup.

```mermaid
%%{init: {'theme':'base','themeVariables':{'fontSize':'18px','primaryColor':'#1565c0','primaryTextColor':'#ffffff','primaryBorderColor':'#0d47a1','lineColor':'#90a4ae','signalColor':'#ffc107','signalTextColor':'#ffc107','actorBkg':'#1565c0','actorBorder':'#0d47a1','actorTextColor':'#ffffff','actorLineColor':'#90caf9','noteBkgColor':'#fff59d','noteTextColor':'#000000','noteBorderColor':'#f57f17','activationBkgColor':'#66bb6a','activationBorderColor':'#2e7d32','sequenceNumberColor':'#ffffff','loopTextColor':'#ffc107','labelBoxBkgColor':'#37474f','labelBoxBorderColor':'#90a4ae','labelTextColor':'#ffffff'},'sequence':{'actorMargin':120,'messageFontSize':17,'noteFontSize':15,'actorFontSize':17,'boxMargin':15,'boxTextMargin':8,'noteMargin':12,'useMaxWidth':false}}}%%
sequenceDiagram
    autonumber
    participant Scr as Screen task
    participant Bdr as Border task
    participant Zmb as Zombie task
    participant Car as Car task

    Note over Scr: SCREEN_ENTRY
    Scr-)Bdr: ZW_GAME_BORDER_SETUP
    activate Bdr
    Note right of Bdr: zw_game_border_clear():<br/>zw_game_score = 0<br/>wave_last_score = 0<br/>wave_warning_timer = 0<br/>wave_warning_active = false<br/>wave_level = 0
    deactivate Bdr
    Note over Scr: arm 100 ms periodic tick

    loop Each ZW_GAME_TIME_TICK
        Note over Zmb,Car: During their own tick handlers,<br/>on bullet/car kill: zw_game_score += 10<br/>(shared global owned by Border)

        Scr-)Bdr: ZW_GAME_BORDER_CHECK_GAME_OVER
        activate Bdr
        loop i = 0 .. ZOMBIE_NUMBER-1
            alt zombie[i].visible == WHITE && zombie[i].x <= -ZOMBIE_MIN_LEFT_OFFSET
                Bdr->>+Car: zw_game_car_find_nearest(zombie[i].y)
                Car-->>-Bdr: idx
                alt idx < 0
                    Bdr-)Scr: ZW_GAME_RESET (AC_TASK_DISPLAY_ID)
                    Note over Bdr: break for-loop
                end
            end
        end
        deactivate Bdr

        Scr-)Bdr: ZW_GAME_BORDER_CHECK_WAVE
        activate Bdr
        alt !wave_warning_active && zw_game_score >= wave_last_score + BORDER_WAVE_SCORE_INTERVAL
            Note right of Bdr: wave_warning_active = true<br/>wave_warning_timer = BORDER_WARNING_BLINK_DURATION
        end
        deactivate Bdr

        Scr-)Bdr: ZW_GAME_BORDER_LEVEL_UP
        activate Bdr
        alt wave_warning_active
            alt wave_warning_timer > 0
                Note right of Bdr: wave_warning_timer--
            else wave_warning_timer == 0
                Note right of Bdr: wave_warning_active = false<br/>wave_last_score += BORDER_WAVE_SCORE_INTERVAL<br/>wave_level++
                Bdr-)Zmb: ZW_GAME_ZOMBIE_WAVE_SPAWN
            end
        end
        deactivate Bdr
    end

    Note over Scr: on ZW_GAME_RESET
    Scr-)Bdr: ZW_GAME_BORDER_RESET
    activate Bdr
    Note right of Bdr: zw_game_border_clear()
    deactivate Bdr
```

<p align="center"><strong><em>Figure 7:</em></strong> Border sequence logic</p>

## IX. Per-Tick Signal Order
                                                                                                                                                                                                                                                                                
The screen task `scr_game_zomwar` posts the following sequence on every `ZW_GAME_TIME_TICK`:

1. `ZW_GAME_GUNNER_UP` or `ZW_GAME_GUNNER_DOWN` (depending on `gunner_dir`)
2. `ZW_GAME_GUNNER_UPDATE`
3. `ZW_GAME_BULLET_RUN`
4. `ZW_GAME_ZOMBIE_RUN`
5. `ZW_GAME_ZOMBIE_DETONATOR`
6. `ZW_GAME_TOMBSTONE_SPAWN`
7. `ZW_GAME_CAR_RUN`
8. `ZW_GAME_CAR_HIT`
9. `ZW_GAME_BANG_UPDATE`
10. `ZW_GAME_BORDER_CHECK_GAME_OVER`
11. `ZW_GAME_BORDER_CHECK_WAVE`
12. `ZW_GAME_BORDER_LEVEL_UP`

On `SCREEN_ENTRY` the screen task posts the matching `*_SETUP` signals to all object tasks and starts the periodic `ZW_GAME_TIME_TICK` timer. On `ZW_GAME_RESET` it removes the timer, posts the `*_RESET` signals, saves `gamescore.score_now = zw_game_score`, transitions to `GAME_OVER`, plays `BUZZER_SOUND_GOODBYE`, and arms a one-shot `ZW_GAME_EXIT_GAME` timer.

## X. Code References

| Object | Source file | Header file |
|---|---|---|
| Gunner | `application/sources/app/game/game_zomwar/zw_game_gunner.cpp` | `application/sources/app/game/game_zomwar/zw_game_gunner.h` |
| Bullet | `application/sources/app/game/game_zomwar/zw_game_bullet.cpp` | `application/sources/app/game/game_zomwar/zw_game_bullet.h` |
| Zombie | `application/sources/app/game/game_zomwar/zw_game_zombie.cpp` | `application/sources/app/game/game_zomwar/zw_game_zombie.h` |
| Bang | `application/sources/app/game/game_zomwar/zw_game_bang.cpp` | `application/sources/app/game/game_zomwar/zw_game_bang.h` |
| Border | `application/sources/app/game/game_zomwar/zw_game_border.cpp` | `application/sources/app/game/game_zomwar/zw_game_border.h` |
| Car | `application/sources/app/game/game_zomwar/zw_game_car.cpp` | `application/sources/app/game/game_zomwar/zw_game_car.h` |
| Tombstone | `application/sources/app/game/game_zomwar/zw_game_tombstone.cpp` | `application/sources/app/game/game_zomwar/zw_game_tombstone.h` |
| Screen | `application/sources/app/screens/scr_game_zomwar.cpp` | `application/sources/app/screens/scr_game_zomwar.h` |
