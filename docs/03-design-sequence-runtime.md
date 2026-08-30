<h1 align="center">Runtime Signal Processing</h1>

This document explains how the Zomwar Game processes button input, task messages, game-loop ticks, and object updates. The game uses the AK event-driven task architecture: each major game object owns a task, receives signals through AK messages, and updates its own state.

## I. Overview

The Zomwar Game is implemented using event-driven tasks.

Each game object owns:

- A dedicated task
- Its own signal handler
- Its own state data
- Its own update logic

The display task (`AC_TASK_DISPLAY_ID`) owns the screen manager and handles screen-level events. During gameplay the active screen `scr_game_zomwar` receives the periodic game tick and posts update messages for each game-object task.

Input events from hardware buttons are converted into software signals. Button callbacks always post `AC_DISPLAY_BUTTON_*` signals to `AC_TASK_DISPLAY_ID`; the active screen handler decides what to do with them. During gameplay the screen translates UP/DOWN into a latched `gunner_dir` and translates MODE_PRESSED into `ZW_GAME_BULLET_SHOOT`.

The main game loop is driven by a periodic timer signal:

```c
ZW_GAME_TIME_TICK
```

The current game tick interval is:

```c
ZW_GAME_TIME_TICK_INTERVAL = 100 ms
```

Main runtime flow:

1. Button callbacks or timers create software signals.
2. Signals are posted into the AK message pool.
3. The AK scheduler dispatches messages to destination task handlers.
4. Each task updates only the state it owns.
5. The screen render reads the latest object state and refreshes the display buffer.

### High Level Architecture

#### 1. Game Start

```mermaid
%%{init: {'theme':'base','themeVariables':{'fontSize':'18px','primaryColor':'#1565c0','primaryTextColor':'#ffffff','primaryBorderColor':'#0d47a1','lineColor':'#90a4ae','signalColor':'#ffc107','signalTextColor':'#ffc107','actorBkg':'#1565c0','actorBorder':'#0d47a1','actorTextColor':'#ffffff','actorLineColor':'#90caf9','noteBkgColor':'#fff59d','noteTextColor':'#000000','noteBorderColor':'#f57f17','activationBkgColor':'#66bb6a','activationBorderColor':'#2e7d32','sequenceNumberColor':'#ffffff','loopTextColor':'#ffc107','labelBoxBkgColor':'#37474f','labelBoxBorderColor':'#90a4ae','labelTextColor':'#ffffff'},'sequence':{'actorMargin':120,'messageFontSize':17,'noteFontSize':15,'actorFontSize':17,'boxMargin':15,'boxTextMargin':8,'noteMargin':12,'useMaxWidth':false}}}%%
sequenceDiagram
    autonumber
    participant Scr as Screen task
    participant EE as EEPROM
    participant Q as AKOS Event-Driven (message pool and Scheduler)
    participant Gun as Gunner task
    participant Bul as Bullet task
    participant Zmb as Zombie task
    participant Car as Car task
    participant Tmb as Tombstone task
    participant Bng as Bang task
    participant Bdr as Border task
    participant Tmr as Timer

    Note over Scr: SCREEN_ENTRY dispatched to scr_game_zomwar
    activate Scr
    Scr->>+EE: zw_game_setting_read(&settingsetup)
    EE-->>-Scr: settings loaded

    Note over Scr,Q: Post 7 SETUP signals (async, RTC defers handlers)
    Scr-)Q: ZW_GAME_GUNNER_SETUP to ZW_GAME_GUNNER_ID
    Scr-)Q: ZW_GAME_BULLET_SETUP to ZW_GAME_BULLET_ID
    Scr-)Q: ZW_GAME_ZOMBIE_SETUP to ZW_GAME_ZOMBIE_ID
    Scr-)Q: ZW_GAME_CAR_SETUP to ZW_GAME_CAR_ID
    Scr-)Q: ZW_GAME_TOMBSTONE_SETUP to ZW_GAME_TOMBSTONE_ID
    Scr-)Q: ZW_GAME_BANG_SETUP to ZW_GAME_BANG_ID
    Scr-)Q: ZW_GAME_BORDER_SETUP to ZW_GAME_BORDER_ID

    Note right of Scr: zw_game_state = GAME_PLAY<br/>gunner_dir = GUNNER_DIR_NONE
    Scr->>Tmr: timer_remove_attr(AC_DISPLAY_SHOW_IDLE)
    Scr->>Tmr: timer_set(ZW_GAME_TIME_TICK, 100 ms, PERIODIC)
    deactivate Scr

    Note over Q: AK scheduler dispatches each queued signal to its task (RTC)
    Q-)Gun: ZW_GAME_GUNNER_SETUP
    activate Gun
    Note right of Gun: init pose (X, Y), visible=WHITE, action_image=1
    deactivate Gun

    Q-)Bul: ZW_GAME_BULLET_SETUP
    activate Bul
    Note right of Bul: clear bullet array (visible=BLACK)
    deactivate Bul

    Q-)Zmb: ZW_GAME_ZOMBIE_SETUP
    activate Zmb
    Note right of Zmb: speed = settingsetup.zombie_speed<br/>spawn NUM_ZOMBIE_INIT zombies at random
    deactivate Zmb

    Q-)Car: ZW_GAME_CAR_SETUP
    activate Car
    Note right of Car: park each lane, visible = bit mask of settingsetup.num_car
    deactivate Car

    Q-)Tmb: ZW_GAME_TOMBSTONE_SETUP
    activate Tmb
    Note right of Tmb: arm spawn_timer<br/>active flags from settingsetup.tombstone_lane_1 and _2
    deactivate Tmb

    Q-)Bng: ZW_GAME_BANG_SETUP
    activate Bng
    Note right of Bng: clear bang array (visible=BLACK)
    deactivate Bng

    Q-)Bdr: ZW_GAME_BORDER_SETUP
    activate Bdr
    Note right of Bdr: zero score, wave_level, wave_last_score<br/>clear warning latch
    deactivate Bdr

    Note over Tmr: 100 ms later
    Tmr-)Scr: ZW_GAME_TIME_TICK (periodic, until ZW_GAME_RESET)
```

<p align="center"><strong><em>Figure 1:</em></strong> Game start sequence logic</p>

#### 2. Game Playing

```mermaid
%%{init: {'theme':'base','themeVariables':{'fontSize':'18px','primaryColor':'#1565c0','primaryTextColor':'#ffffff','primaryBorderColor':'#0d47a1','lineColor':'#90a4ae','signalColor':'#ffc107','signalTextColor':'#ffc107','actorBkg':'#1565c0','actorBorder':'#0d47a1','actorTextColor':'#ffffff','actorLineColor':'#90caf9','noteBkgColor':'#fff59d','noteTextColor':'#000000','noteBorderColor':'#f57f17','activationBkgColor':'#66bb6a','activationBorderColor':'#2e7d32','sequenceNumberColor':'#ffffff','loopTextColor':'#ffc107','labelBoxBkgColor':'#37474f','labelBoxBorderColor':'#90a4ae','labelTextColor':'#ffffff'},'sequence':{'actorMargin':120,'messageFontSize':17,'noteFontSize':15,'actorFontSize':17,'boxMargin':15,'boxTextMargin':8,'noteMargin':12,'useMaxWidth':false}}}%%
sequenceDiagram
    autonumber
    actor Btn as Button
    participant Tmr as Timer
    participant Q as AKOS Event-Driven (message pool and Scheduler)
    participant Scr as Screen task
    participant Gun as Gunner task
    participant Bul as Bullet task
    participant Zmb as Zombie task
    participant Tmb as Tombstone task
    participant Car as Car task
    participant Bng as Bang task
    participant Bdr as Border task

    Note over Btn,Scr: Button events fire asynchronously between ticks
    Btn-)Q: AC_DISPLAY_BUTTON_UP_PRESSED to AC_TASK_DISPLAY_ID
    Q-)Scr: dispatch
    activate Scr
    Note right of Scr: gunner_dir = GUNNER_DIR_UP
    deactivate Scr

    Btn-)Q: AC_DISPLAY_BUTTON_UP_RELEASED
    Q-)Scr: dispatch
    activate Scr
    Note right of Scr: if dir == GUNNER_DIR_UP, dir = GUNNER_DIR_NONE
    deactivate Scr

    Btn-)Q: AC_DISPLAY_BUTTON_MODE_PRESSED
    Q-)Scr: dispatch
    activate Scr
    opt zw_game_state == GAME_PLAY
        Scr-)Q: ZW_GAME_BULLET_SHOOT to ZW_GAME_BULLET_ID
    end
    deactivate Scr
    Q-)Bul: ZW_GAME_BULLET_SHOOT (dispatched after Screen RTC)
    activate Bul
    Note right of Bul: spawn bullet, set gunner.action_image=2, play CLICK
    deactivate Bul

    Note over Tmr,Scr: Periodic tick (100 ms)
    Tmr-)Q: ZW_GAME_TIME_TICK to AC_TASK_DISPLAY_ID
    Q-)Scr: dispatch
    activate Scr
    opt zw_game_state != GAME_PLAY
        Note right of Scr: break (ignore tick)
    end

    alt gunner_dir == GUNNER_DIR_UP
        Scr-)Q: ZW_GAME_GUNNER_UP
    else gunner_dir == GUNNER_DIR_DOWN
        Scr-)Q: ZW_GAME_GUNNER_DOWN
    else gunner_dir == GUNNER_DIR_NONE
        Note right of Scr: skip movement post
    end
    Scr-)Q: ZW_GAME_GUNNER_UPDATE
    Scr-)Q: ZW_GAME_BULLET_RUN
    Scr-)Q: ZW_GAME_ZOMBIE_RUN
    Scr-)Q: ZW_GAME_ZOMBIE_DETONATOR
    Scr-)Q: ZW_GAME_TOMBSTONE_SPAWN
    Scr-)Q: ZW_GAME_CAR_RUN
    Scr-)Q: ZW_GAME_CAR_HIT
    Scr-)Q: ZW_GAME_BANG_UPDATE
    Scr-)Q: ZW_GAME_BORDER_CHECK_GAME_OVER
    Scr-)Q: ZW_GAME_BORDER_CHECK_WAVE
    Scr-)Q: ZW_GAME_BORDER_LEVEL_UP
    deactivate Scr

    Note over Q: AK scheduler dispatches each queued signal (RTC, FIFO)

    Q-)Gun: ZW_GAME_GUNNER_UP or ZW_GAME_GUNNER_DOWN
    activate Gun
    Note right of Gun: gunner_y += or -= STEP, clamp
    deactivate Gun

    Q-)Gun: ZW_GAME_GUNNER_UPDATE
    activate Gun
    Note right of Gun: gunner.y from gunner_y, clear recoil
    deactivate Gun

    Q-)Bul: ZW_GAME_BULLET_RUN
    activate Bul
    Note right of Bul: advance bullets, despawn at right edge
    deactivate Bul

    Q-)Zmb: ZW_GAME_ZOMBIE_RUN
    activate Zmb
    Note right of Zmb: rise or step-left with zigzag, animate, top-up alive count
    deactivate Zmb

    Q-)Zmb: ZW_GAME_ZOMBIE_DETONATOR
    activate Zmb
    Note right of Zmb: bullet vs zombie hit, write bang and score, BUZZER, hide
    deactivate Zmb

    Q-)Tmb: ZW_GAME_TOMBSTONE_SPAWN
    activate Tmb
    Note right of Tmb: countdown spawn_timer, on expire spawn rising zombie via sync helper
    deactivate Tmb

    Q-)Car: ZW_GAME_CAR_RUN
    activate Car
    Note right of Car: activation pass plus advance running cars
    deactivate Car

    Q-)Car: ZW_GAME_CAR_HIT
    activate Car
    Note right of Car: running car vs zombie, write bang and score, BUZZER, hide
    deactivate Car

    Q-)Bng: ZW_GAME_BANG_UPDATE
    activate Bng
    Note right of Bng: animate visible slots 1->2->3, retire at 3
    deactivate Bng

    Q-)Bdr: ZW_GAME_BORDER_CHECK_GAME_OVER
    activate Bdr
    loop scan visible zombies past left edge
        Bdr->>+Car: zw_game_car_find_nearest(z.y)
        Car-->>-Bdr: idx
        opt idx less than 0
            Bdr-)Q: ZW_GAME_RESET to AC_TASK_DISPLAY_ID
            Note right of Bdr: break loop
        end
    end
    deactivate Bdr

    Q-)Bdr: ZW_GAME_BORDER_CHECK_WAVE
    activate Bdr
    Note right of Bdr: arm wave warning if score crossed threshold
    deactivate Bdr

    Q-)Bdr: ZW_GAME_BORDER_LEVEL_UP
    activate Bdr
    opt warning active and timer reached 0
        Note right of Bdr: advance wave_last_score, wave_level++
        Bdr-)Q: ZW_GAME_ZOMBIE_WAVE_SPAWN to ZW_GAME_ZOMBIE_ID
    end
    deactivate Bdr
```

<p align="center"><strong><em>Figure 2:</em></strong> Gameplay sequence logic</p>

#### 3. Game Reset

```mermaid
%%{init: {'theme':'base','themeVariables':{'fontSize':'18px','primaryColor':'#1565c0','primaryTextColor':'#ffffff','primaryBorderColor':'#0d47a1','lineColor':'#90a4ae','signalColor':'#ffc107','signalTextColor':'#ffc107','actorBkg':'#1565c0','actorBorder':'#0d47a1','actorTextColor':'#ffffff','actorLineColor':'#90caf9','noteBkgColor':'#fff59d','noteTextColor':'#000000','noteBorderColor':'#f57f17','activationBkgColor':'#66bb6a','activationBorderColor':'#2e7d32','sequenceNumberColor':'#ffffff','loopTextColor':'#ffc107','labelBoxBkgColor':'#37474f','labelBoxBorderColor':'#90a4ae','labelTextColor':'#ffffff'},'sequence':{'actorMargin':120,'messageFontSize':17,'noteFontSize':15,'actorFontSize':17,'boxMargin':15,'boxTextMargin':8,'noteMargin':12,'useMaxWidth':false}}}%%
sequenceDiagram
    autonumber
    participant Bdr as Border task
    participant Q as AKOS Event-Driven (message pool and Scheduler)
    participant Tmr as Timer
    participant Scr as Screen task
    participant Gun as Gunner task
    participant Bul as Bullet task
    participant Zmb as Zombie task
    participant Car as Car task
    participant Tmb as Tombstone task
    participant Bng as Bang task
    participant Bz as Buzzer

    Note over Bdr: trigger from ZW_GAME_BORDER_CHECK_GAME_OVER<br/>zombie past left edge with no rescue car nearby
    Bdr-)Q: ZW_GAME_RESET to AC_TASK_DISPLAY_ID
    Q-)Scr: dispatch
    activate Scr
    opt zw_game_state != GAME_PLAY
        Note right of Scr: break (already over)
    end
    Scr->>Tmr: timer_remove_attr(ZW_GAME_TIME_TICK)
    Note right of Scr: periodic tick stops

    Note over Scr,Q: Fan out 7 RESET signals (async)
    Scr-)Q: ZW_GAME_GUNNER_RESET to ZW_GAME_GUNNER_ID
    Scr-)Q: ZW_GAME_BULLET_RESET to ZW_GAME_BULLET_ID
    Scr-)Q: ZW_GAME_ZOMBIE_RESET to ZW_GAME_ZOMBIE_ID
    Scr-)Q: ZW_GAME_CAR_RESET to ZW_GAME_CAR_ID
    Scr-)Q: ZW_GAME_TOMBSTONE_RESET to ZW_GAME_TOMBSTONE_ID
    Scr-)Q: ZW_GAME_BANG_RESET to ZW_GAME_BANG_ID
    Scr-)Q: ZW_GAME_BORDER_RESET to ZW_GAME_BORDER_ID

    Note right of Scr: gamescore.score_now = zw_game_score (latch for game-over screen)<br/>zw_game_state = GAME_OVER
    Scr->>+Bz: BUZZER_PlaySound(BUZZER_SOUND_GOODBYE)
    Bz-->>-Scr: 
    Scr->>Tmr: timer_set(ZW_GAME_EXIT_GAME, 3000 ms, ONE_SHOT)
    deactivate Scr

    Note over Q: AK dispatches each RESET in FIFO order (RTC)
    Q-)Gun: ZW_GAME_GUNNER_RESET
    activate Gun
    Note right of Gun: re-park, visible=BLACK
    deactivate Gun

    Q-)Bul: ZW_GAME_BULLET_RESET
    activate Bul
    Note right of Bul: clear bullet array
    deactivate Bul

    Q-)Zmb: ZW_GAME_ZOMBIE_RESET
    activate Zmb
    Note right of Zmb: hide all zombies
    deactivate Zmb

    Q-)Car: ZW_GAME_CAR_RESET
    activate Car
    Note right of Car: re-park lanes, visible=false, running=false
    deactivate Car

    Q-)Tmb: ZW_GAME_TOMBSTONE_RESET
    activate Tmb
    Note right of Tmb: clear timer and slots
    deactivate Tmb

    Q-)Bng: ZW_GAME_BANG_RESET
    activate Bng
    Note right of Bng: clear bang array
    deactivate Bng

    Q-)Bdr: ZW_GAME_BORDER_RESET
    activate Bdr
    Note right of Bdr: zw_game_border_clear()<br/>zero score, wave_level, warning latch
    deactivate Bdr

    Note over Tmr: 3 s later
    Tmr-)Q: ZW_GAME_EXIT_GAME to AC_TASK_DISPLAY_ID
    Q-)Scr: dispatch
    activate Scr
    Note right of Scr: zw_game_state = GAME_OFF<br/>SCREEN_TRAN to scr_game_over_handle
    deactivate Scr
```

<p align="center"><strong><em>Figure 3:</em></strong> Game reset sequence logic</p>

## II. Code References

| Area | File |
|---|---|
| Task IDs and task handlers | `application/sources/app/task_list.h` |
| Task table registration | `application/sources/app/task_list.cpp` |
| Signal definitions | `application/sources/app/app.h` |
| Button callback logic | `application/sources/app/app_bsp.cpp` |
| Main game screen logic | `application/sources/app/screens/scr_game_zomwar.cpp` |
| Game-over screen logic | `application/sources/app/screens/scr_game_over.cpp` |
| Screen manager | `application/sources/common/screen_manager.cpp` |

## III. Task Ownership

| Task | Responsibility | Owns Data | Receives Signals |
|---|---|---|---|
| `AC_TASK_DISPLAY_ID` | Screen manager, render scheduling, button routing, central game-tick dispatch | Current screen state, `zw_game_state`, `gunner_dir`, `gamescore` | All `AC_DISPLAY_*` button signals, `ZW_GAME_TIME_TICK`, `ZW_GAME_RESET`, `ZW_GAME_EXIT_GAME` |
| `ZW_GAME_GUNNER_ID` | Player control | `gunner`, internal `gunner_y` | `ZW_GAME_GUNNER_SETUP`, `ZW_GAME_GUNNER_UPDATE`, `ZW_GAME_GUNNER_UP`, `ZW_GAME_GUNNER_DOWN`, `ZW_GAME_GUNNER_RESET` |
| `ZW_GAME_BULLET_ID` | Bullet movement and shooting | `bullet[]` | `ZW_GAME_BULLET_SETUP`, `ZW_GAME_BULLET_RUN`, `ZW_GAME_BULLET_SHOOT`, `ZW_GAME_BULLET_RESET` |
| `ZW_GAME_ZOMBIE_ID` | Zombie movement, collision, wave spawn, menu demo | `zombie[]`, `zw_game_zombie_speed` | `ZW_GAME_ZOMBIE_SETUP`, `ZW_GAME_ZOMBIE_RUN`, `ZW_GAME_ZOMBIE_DETONATOR`, `ZW_GAME_ZOMBIE_WAVE_SPAWN`, `ZW_GAME_ZOMBIE_RESET`, `ZW_GAME_ZOMBIE_SETUP_MENU`, `ZW_GAME_ZOMBIE_RUN_MENU` |
| `ZW_GAME_CAR_ID` | Lawnmower rescue logic | `car[]` | `ZW_GAME_CAR_SETUP`, `ZW_GAME_CAR_RUN`, `ZW_GAME_CAR_HIT`, `ZW_GAME_CAR_RESET` |
| `ZW_GAME_TOMBSTONE_ID` | Tombstone spawn timing | `tombstone[]`, `tombstone_spawn_timer` | `ZW_GAME_TOMBSTONE_SETUP`, `ZW_GAME_TOMBSTONE_SPAWN`, `ZW_GAME_TOMBSTONE_RESET` |
| `ZW_GAME_BANG_ID` | Explosion animation | `bang[]` | `ZW_GAME_BANG_SETUP`, `ZW_GAME_BANG_UPDATE`, `ZW_GAME_BANG_RESET` |
| `ZW_GAME_BORDER_ID` | Wave / level-up / game-over detection | `zw_game_score`, `wave_last_score`, `wave_level`, `wave_warning_active`, `wave_warning_timer` | `ZW_GAME_BORDER_SETUP`, `ZW_GAME_BORDER_CHECK_GAME_OVER`, `ZW_GAME_BORDER_CHECK_WAVE`, `ZW_GAME_BORDER_LEVEL_UP`, `ZW_GAME_BORDER_RESET` |

## IV. Button Event Processing

In Zomwar, button callbacks always post `AC_DISPLAY_BUTTON_*` signals to `AC_TASK_DISPLAY_ID`. The currently active screen handler then decides what to do with them. The Zomwar gameplay screen (`scr_game_zomwar`) handles those signals locally; it does not require the BSP to know which screen is active.

### Button Processing Rules

| Button | BSP posts to AK | Active screen | Result inside the screen handler |
|---|---|---|---|
| MODE Pressed | `AC_DISPLAY_BUTTON_MODE_PRESSED` -> `AC_TASK_DISPLAY_ID` | `scr_game_zomwar` | If `zw_game_state == GAME_PLAY`: post `ZW_GAME_BULLET_SHOOT` to `ZW_GAME_BULLET_ID` |
| UP Pressed | `AC_DISPLAY_BUTTON_UP_PRESSED` -> `AC_TASK_DISPLAY_ID` | `scr_game_zomwar` | `gunner_dir = GUNNER_DIR_UP` (no message posted) |
| UP Released | `AC_DISPLAY_BUTTON_UP_RELEASED` -> `AC_TASK_DISPLAY_ID` | `scr_game_zomwar` | If `gunner_dir == GUNNER_DIR_UP`: `gunner_dir = GUNNER_DIR_NONE` |
| DOWN Pressed | `AC_DISPLAY_BUTTON_DOWN_PRESSED` -> `AC_TASK_DISPLAY_ID` | `scr_game_zomwar` | `gunner_dir = GUNNER_DIR_DOWN` (no message posted) |
| DOWN Released | `AC_DISPLAY_BUTTON_DOWN_RELEASED` -> `AC_TASK_DISPLAY_ID` | `scr_game_zomwar` | If `gunner_dir == GUNNER_DIR_DOWN`: `gunner_dir = GUNNER_DIR_NONE` |

Note: the BSP's MODE callback only posts on `BUTTON_SW_STATE_PRESSED`; there is no `MODE_RELEASED` event. The actual `ZW_GAME_GUNNER_UP`/`DOWN` signals are posted by the screen on every `ZW_GAME_TIME_TICK` based on the latched `gunner_dir`, not by the button callback directly.

## V. Runtime Scheduling Notes

The system uses asynchronous task-based scheduling.

Important runtime characteristics:

- Signals are queued in AK before they are handled.
- A sender posts a message; it does not directly execute the destination task handler.
- Handlers run to completion (RTC): a task processes one signal fully before pulling the next.
- Game logic is decoupled through signals.
- Timing depends on scheduler execution order.
- A signal may wait in the AK message pool before its handler executes.
- `ZW_GAME_TIME_TICK` can appear between button pressed and released logs because the timer keeps running.
- Bang has no spawn signal: Zombie's `ZW_GAME_ZOMBIE_DETONATOR` and Car's `ZW_GAME_CAR_RUN`/`ZW_GAME_CAR_HIT` call `zw_game_bang_spawn()` directly to write into `bang[]`. The Bang task only owns the per-tick animation (`ZW_GAME_BANG_UPDATE`).
- Border crosses task boundaries in two places: it posts `ZW_GAME_ZOMBIE_WAVE_SPAWN` to Zombie on `ZW_GAME_BORDER_LEVEL_UP` (async), and calls `zw_game_car_find_nearest()` synchronously into the Car module on `ZW_GAME_BORDER_CHECK_GAME_OVER` (sync helper, reads `car[]`).
- Tombstone writes into `zombie[]` via a synchronous helper `zw_game_zombie_spawn_from_tombstone()` during its own `ZW_GAME_TOMBSTONE_SPAWN` tick; it does not post a signal to the Zombie task.
- The game-over flow is split between two screens: `scr_game_zomwar` latches `gamescore.score_now`, plays `BUZZER_SOUND_GOODBYE`, and arms a one-shot `ZW_GAME_EXIT_GAME` timer; `scr_game_over` runs the ranking and writes the score to EEPROM only after the user presses Retry / Rank / Home.
