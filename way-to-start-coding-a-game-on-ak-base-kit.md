# AK-Base-Kit STM32L151 — Hướng Dẫn Bắt Đầu Lập Trình Game

Chào mừng bạn đến với dự án phát triển Game trên vi điều khiển STM32L151! Repository này cung cấp bộ source code nền tảng cùng tài liệu hướng dẫn chi tiết, giúp bạn nhanh chóng làm quen với kiến trúc hệ thống và bắt tay vào lập trình tựa game của riêng mình.

---

## Mục lục

- [I. Tạo "Sân chơi riêng" (Fork)](#i-tạo-sân-chơi-riêng-fork)
- [II. Setup môi trường Build & Nạp kit](#ii-setup-môi-trường-build--nạp-kit)
- [III. Quy trình lập trình game hằng ngày](#iii-quy-trình-lập-trình-game-hằng-ngày)
  - [Bước 0: Tạo thư mục làm việc](#bước-0-tạo-thư-mục-làm-việc)
  - [Bước 1: Clone repo về máy](#bước-1-clone-repo-về-máy)
  - [Bước 2: Modify Game](#bước-2-modify-game)
  - [Bước 3: Push code lên GitHub](#bước-3-push-code-lên-github)

---

## I. Tạo "Sân chơi riêng" (Fork)

Để bắt đầu dự án cá nhân, bạn thực hiện theo các bước sau:

### 1. Truy cập repository gốc

**Link:** [https://github.com/the-ak-foundation/ak-base-kit-stm32l151](https://github.com/the-ak-foundation/ak-base-kit-stm32l151)

### 2. Fork repository

Nhấn nút **Fork** ở góc trên bên phải để copy dự án về tài khoản cá nhân của bạn.

<p align="center">
  <img width="900" alt="Fork repository" src="https://github.com/user-attachments/assets/591ff540-c556-4fdc-9fec-e1974bd7cdd2" />
</p>

### 3. Tạo bản fork

<p align="center">
  <img width="500" alt="Create fork" src="https://github.com/user-attachments/assets/378721d5-26c5-4898-b523-7e07594f6583" />
</p>

> **Lưu ý:**
> - Tên repository sẽ là **tên game** của bạn.
> - Bạn có thể mô tả về game của mình trong phần **Description**.

Sau khi tạo thành công, GitHub sẽ hiển thị repository fork — đây là "bộ khung" cơ bản để bạn phát triển và hoàn thiện game:

<p align="center">
  <img width="900" alt="Forked repository" src="https://github.com/user-attachments/assets/37821fde-90e9-4c4e-babb-2c7dc03fa07c" />
</p>

---

## II. Setup môi trường Build & Nạp kit

Vui lòng làm theo hướng dẫn chi tiết tại đây:

**[AK Embedded Base Kit STM32L151 — Getting Started](https://epcb.vn/blogs/ak-embedded-software/ak-embedded-base-kit-stm32l151-getting-started)**

---

## III. Quy trình lập trình game

> **Quan trọng:** Vì đây là dự án phần mềm nhúng, hãy sử dụng **Terminal trên môi trường Ubuntu/Linux** để đảm bảo tính chính xác.

### Bước 1: Tạo thư mục làm việc

Từ thư mục `Home`, tạo một thư mục **Workspace** — đây là nơi chứa toàn bộ source code và công cụ lập trình.

<p align="center">
  <img width="800" alt="Tạo thư mục Workspace" src="https://github.com/user-attachments/assets/60eff60e-e2d2-4f8e-9183-c67934ea0a28" />
</p>

Bên trong `Workspace`, tạo 2 thư mục con:

| Thư mục | Mục đích |
|---------|----------|
| `Sources` | Chứa các dự án lập trình của bạn |
| `Tools` | Chứa các công cụ lập trình (xem chi tiết tại [phần II](#ii-setup-môi-trường-build--nạp-kit)) |

<p align="center">
  <img width="900" alt="Cấu trúc Workspace" src="https://github.com/user-attachments/assets/b1fa5a0f-ef28-4eba-b823-bec7030117fb" />
</p>

---

### Bước 2: Clone repo về máy

> *Bước này chỉ thực hiện **một lần duy nhất** khi bắt đầu dự án.*

Mở **Terminal** trong thư mục `Sources` và chạy lệnh sau (nhớ thay bằng link repo của bạn):

```bash
git clone https://github.com/<ten-cua-ban>/<ten-repo-da-clone>.git
```

<p align="center">
  <img width="900" alt="Clone repository" src="https://github.com/user-attachments/assets/6ba6e15d-ce85-483e-9519-61fcb017dafb" />
</p>

---

### Bước 3: Modify Game

Mở **VSCode** trên Linux và mở repository vừa clone để bắt đầu lập trình.
s
> Toàn bộ logic game được viết trong thư mục `application/`.

<p align="center">
  <img width="900" alt="Cấu trúc thư mục application" src="https://github.com/user-attachments/assets/c403a5a8-80c3-42fc-a96f-499c05c92a73" />
</p>

#### Ví dụ: Hiển thị màn hình Peashooter (Cây đậu bắn súng trong) trong Plants and Zombies

**Bước 3.1 —** Tạo file header trong thư mục `screens/`:

<p align="center">
  <img width="876" height="569" alt="image" src="https://github.com/user-attachments/assets/aa947653-4c3f-4bb5-93dc-5ad8a2048968" />
</p>

**Bước 3.2 —** Tạo file lưu trữ và xử lý dữ liệu bitmap của Peashooter cho màn hình hiển thị:

<p align="center">
  <img width="1274" height="835" alt="image" src="https://github.com/user-attachments/assets/39eda4fc-1f14-4ab8-b3f4-96a2b6893b24" />
</p>

**Bước 3.3 —** Tạo file header trong thư mục `screens/`:

<p align="center">
  <img width="845" height="565" alt="image" src="https://github.com/user-attachments/assets/ec495618-9890-4ee6-9835-af274cb504cd" />
</p>

**Bước 3.4 —** Tạo file xử lý dữ liệu bitmap của Peashooter cho màn hình hiển thị:

<p align="center">
  <img width="976" height="713" alt="image" src="https://github.com/user-attachments/assets/cd858d8d-046c-4c3e-bf0c-2c84fbd66123" />
</p>

**Bước 3.5 —** Include file header của Peashooter vào task_display.h:
<p align="center">
  <img width="961" height="715" alt="image" src="https://github.com/user-attachments/assets/5e6d7167-7915-4335-847a-f758d74cc6ad" />
</p>

**Bước 3.6 —** Cập nhật lại case AC_DISPLAY_BUTON_MODE_RELEASED:
<p align="center">
  <img width="1058" height="557" alt="image" src="https://github.com/user-attachments/assets/d020678d-ca86-46ea-bdfd-23a08d98394b" />
</p>

**Bước 3.7 —** Thêm 2 file scr_peashooter.cpp và screens_bitmap.cpp vào Makefile.mk trong thư mục `screens/` để compile:
<p align="center">
<img width="887" height="536" alt="image" src="https://github.com/user-attachments/assets/346fa96e-a896-4010-9fdc-462566ea37a4" />
</p>

**Bước 3.8 —** Build và nạp firmware lên kit (xem hướng dẫn chi tiết tại [phần II](#ii-setup-môi-trường-build--nạp-kit)):

<p align="center">
  <img width="2540" height="2562" alt="image" src="https://github.com/user-attachments/assets/513e18e3-4141-418a-a290-2bc45f97b54e" />
</p>

---

### Bước 4: Push code lên GitHub

Sau khi hoàn thành một tính năng, lưu lại tiến độ lên repo cá nhân bằng các lệnh sau (chạy tại **thư mục gốc** của repo):

```bash
git add .
git commit -m "Update logo AK Foundation"
git push origin main
```

**Kết quả sau khi chạy lệnh:**

<p align="center">
  <img width="800" alt="Kết quả git push" src="https://github.com/user-attachments/assets/ff8edf7a-e368-42c2-82dc-123d16843b01" />
</p>

**Repository đã được cập nhật trên GitHub:**

<p align="center">
  <img width="900" alt="Repository đã cập nhật" src="https://github.com/user-attachments/assets/d09a9419-0d1b-47d8-9148-ef494f58932a" />
</p>

> Sau bước này, mọi người có thể truy cập trực tiếp vào link GitHub của bạn để xem tiến độ và trải nghiệm game bạn đã viết.

---

## Liên hệ & Hỗ trợ

- Website: [epcb.vn](https://epcb.vn)
- Blog: [AK Embedded Software](https://epcb.vn/blogs/ak-embedded-software)

---

<p align="center">
  <i>Chúc bạn lập trình vui vẻ và tạo ra những tựa game thú vị!</i>
</p>
