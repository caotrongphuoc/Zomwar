# Hướng Dẫn Bắt Đầu Lập Trình Game

Chào mừng bạn đến với dự án lập trình Game trên vi điều khiển STM32L151! Repository này cung cấp bộ source code nền tảng cùng tài liệu hướng dẫn chi tiết, giúp bạn nhanh chóng làm quen với kiến trúc hệ thống và bắt tay vào phát triển tựa game của riêng mình.

---

## Mục lục

- [I. Tạo "Sân chơi riêng" (Fork)](#i-tạo-sân-chơi-riêng-fork)
- [II. Setup môi trường Build & Nạp kit](#ii-setup-môi-trường-build--nạp-kit)
- [III. Quy trình lập trình game](#iii-quy-trình-lập-trình-game)
  - [Bước 1: Tạo thư mục làm việc](#bước-1-tạo-thư-mục-làm-việc)
  - [Bước 2: Clone repo về máy](#bước-2-clone-repo-về-máy)
  - [Bước 3: Modify Game](#bước-3-modify-game)
  - [Bước 4: Push code lên GitHub](#bước-4-push-code-lên-github)

---

## I. Tạo "Sân chơi riêng" (Fork)

Để khởi tạo dự án cá nhân, bạn thực hiện theo các bước sau:

### 1. Truy cập repository gốc

**Link:** [https://github.com/the-ak-foundation/ak-base-kit-stm32l151](https://github.com/the-ak-foundation/ak-base-kit-stm32l151)

### 2. Fork repository

Nhấn nút **Fork** ở góc trên bên phải để tạo một bản sao dự án về tài khoản cá nhân của bạn.
Ngoài ra, các bạn có thể nhấn nút **Starred** ở bên phải nút **Fork** để ủng hộ tác giả nhé.

<p align="center">
  <img width="1224" height="775" alt="Screenshot from 2026-04-29 11-53-32(1)" src="https://github.com/user-attachments/assets/620b1396-a728-46f2-884f-a788533c4d69" />
</p>

### 3. Tạo bản fork

<p align="center">
  <img width="500" alt="Create fork" src="https://github.com/user-attachments/assets/378721d5-26c5-4898-b523-7e07594f6583" />
</p>

> **Note:**
> - Đặt tên repository chính là **tên game** của bạn.
> - Mô tả ngắn gọn về game trong phần **Description**.

Sau khi fork thành công, GitHub sẽ chuyển đến repository mới — đây chính là "bộ khung" để bạn phát triển và hoàn thiện game:

<p align="center">
  <img width="1224" height="785" alt="image" src="https://github.com/user-attachments/assets/614fafce-3090-4be0-9880-3c7dc7d6f979" />
</p>

---

## II. Quick Start Guide (Setup môi trường)

Để build source code và nạp firmware lên kit, bạn cần cài đặt môi trường phát triển trên Ubuntu/Linux. Hướng dẫn chi tiết từng bước có tại:

**[AK Embedded Base Kit STM32L151 — Getting Started](https://epcb.vn/blogs/ak-embedded-software/ak-embedded-base-kit-stm32l151-getting-started)**

---

## III. Quy trình lập trình game

> **Note:** Vì đây là dự án phần mềm nhúng, bạn nên sử dụng **Terminal trên môi trường Ubuntu/Linux** để đảm bảo quá trình build và nạp firmware diễn ra chính xác.

### Bước 1: Tạo thư mục làm việc

Từ thư mục `Home`, tạo một thư mục đặt tên là **Workspace** — đây sẽ là nơi chứa toàn bộ source code và công cụ lập trình.

<p align="center">
  <img width="800" alt="Tạo thư mục Workspace" src="https://github.com/user-attachments/assets/60eff60e-e2d2-4f8e-9183-c67934ea0a28" />
</p>

Bên trong `Workspace`, tạo thêm 2 thư mục con:

| Thư mục   | Mục đích                                                                          |
| --------- | --------------------------------------------------------------------------------- |
| `Sources` | Chứa các dự án lập trình của bạn                                                  |
| `Tools`   | Chứa các công cụ lập trình (xem chi tiết tại [phần II](#ii-setup-môi-trường-build--nạp-kit)) |

<p align="center">
  <img width="900" alt="Cấu trúc Workspace" src="https://github.com/user-attachments/assets/b1fa5a0f-ef28-4eba-b823-bec7030117fb" />
</p>

---

### Bước 2: Clone repo về máy

> **Note:** Bước này chỉ cần thực hiện **một lần duy nhất** khi bắt đầu dự án.

Mở **Terminal** ngay tại thư mục `Sources` và chạy lệnh sau (nhớ thay bằng link repo của bạn):

```bash
git clone https://github.com/<ten-cua-ban>/<ten-repo-da-clone>.git
```

<p align="center">
  <img width="900" alt="Clone repository" src="https://github.com/user-attachments/assets/6ba6e15d-ce85-483e-9519-61fcb017dafb" />
</p>

---

### Bước 3: Modify Game

Mở **VSCode** trên Linux, sau đó mở repository vừa clone để bắt đầu lập trình.

Toàn bộ logic game được viết trong thư mục `application/`.

<p align="center">
  <img width="1224" height="785" alt="image" src="https://github.com/user-attachments/assets/3a4ab383-6fac-486b-8f18-67afc3521864" />
</p>

#### Ví dụ: Hiển thị màn hình Peashooter (Cây đậu bắn súng) trong game Plants vs. Zombies

**Bước 3.1 —** Tạo file header `scr_peashooter.h` trong thư mục `screens/` để khai báo các hàm vẽ màn hình Peashooter:

<p align="center">
  <img width="1224" height="785" alt="image" src="https://github.com/user-attachments/assets/5c3e469b-468d-4273-aea7-30202b3596af" />
</p>

**Bước 3.2 —** Tạo file `scr_peashooter.cpp` để xử lý dữ liệu bitmap và hiển thị Peashooter lên màn hình:

<p align="center">
  <img width="1224" height="785" alt="image" src="https://github.com/user-attachments/assets/3a496f51-ec9a-4bc1-beb8-35baae534a42" />
</p>

**Bước 3.3 —** Tạo file header `screens_bitmap.h` trong thư mục `screens/` để khai báo dữ liệu bitmap dùng chung:

<p align="center">
  <img width="1224" height="785" alt="image" src="https://github.com/user-attachments/assets/d6ea016c-bfc8-4e0c-bda3-4c15a65fad20" />
</p>

**Bước 3.4 —** Tạo file `screens_bitmap.cpp` chứa dữ liệu bitmap của Peashooter:

<p align="center">
  <img width="1224" height="785" alt="image" src="https://github.com/user-attachments/assets/23ebde81-f3d2-4dfb-87f9-20a2aa3fc5a2" />
</p>

**Bước 3.5 —** Include file header của Peashooter vào `task_display.h`:

<p align="center">
  <img width="1224" height="785" alt="image" src="https://github.com/user-attachments/assets/7fe8c914-5d52-4e23-afeb-e3d936b25502" />
</p>

**Bước 3.6 —** Cập nhật lại case `AC_DISPLAY_BUTTON_MODE_RELEASED`:

<p align="center">
  <img width="1224" height="785" alt="abc" src="https://github.com/user-attachments/assets/7f491cf1-d9e9-4fcb-bd11-7db6c3f24f43" />
</p>

**Bước 3.7 —** Thêm hai file `scr_peashooter.cpp` và `screens_bitmap.cpp` vào `Makefile.mk` trong thư mục `screens/` để biên dịch:

<p align="center">
  <img width="1224" height="785" alt="image" src="https://github.com/user-attachments/assets/c96aeceb-d40c-4552-bfd5-03f7cff28283" />
</p>

**Bước 3.8 —** Build và nạp firmware lên kit (xem hướng dẫn chi tiết tại [phần II](#ii-setup-môi-trường-build--nạp-kit)):

<p align="center">
  <img width="2540" height="2562" alt="image" src="https://github.com/user-attachments/assets/513e18e3-4141-418a-a290-2bc45f97b54e" />
</p>

---

### Bước 4: Push code lên GitHub

Sau khi hoàn thành một tính năng, hãy lưu lại tiến độ lên repo cá nhân bằng các lệnh sau (chạy tại **thư mục gốc** của repo):

```bash
git add .
git commit -m "Create screen of Peashooter"
git push origin main
```

**Kết quả sau khi chạy lệnh:**

<p align="center">
  <img width="609" height="195" alt="image" src="https://github.com/user-attachments/assets/b3a51ad7-c300-4749-bcf4-ab5d702d168b" />
</p>

**Repository đã được cập nhật trên GitHub:**

<p align="center">
  <img width="1224" height="785" alt="Screenshot from 2026-04-29 11-49-23" src="https://github.com/user-attachments/assets/fdb79fe0-d4b9-4625-bd3e-ca3216a9a633" />
</p>

<p align="center">
<img width="1224" height="785" alt="image" src="https://github.com/user-attachments/assets/417de700-df85-42b6-8f61-7816536142af" />
</p>

Sau bước này, bất kỳ ai cũng có thể truy cập link GitHub của bạn để theo dõi tiến độ và trải nghiệm tựa game mà bạn đã tạo ra.

---

## Liên hệ & Hỗ trợ

- Linkedin: www.linkedin.com/in/cao-trong-phuoc
- Dép lào: 0936310918

## Tài liệu tham khảo
- Blog: [AK Embedded Software](https://epcb.vn/blogs/ak-embedded-software)

---

<p align="center">
  <i>Chúc bạn lập trình vui vẻ và tạo ra những tựa game thật thú vị!</i>
</p>
