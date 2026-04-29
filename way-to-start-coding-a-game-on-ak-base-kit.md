# Hướng Dẫn Bắt Đầu Lập Trình Game

Chào mừng bạn đến với dự án lập trình Game trên vi điều khiển STM32L151! Repository này cung cấp bộ source code nền tảng cùng tài liệu hướng dẫn chi tiết, giúp bạn nhanh chóng làm quen với kiến trúc hệ thống và bắt tay vào phát triển tựa game của riêng mình.

---

## Mục lục

- [I. Tạo "Sân chơi riêng" (Fork)](#i-tạo-sân-chơi-riêng-fork)
- [II. Quick Start Guide (Setup môi trường)](#ii-quick-start-guide-setup-môi-trường)
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
  <img src="https://github.com/user-attachments/assets/620b1396-a728-46f2-884f-a788533c4d69" alt="Fork repository" style="width:900px; height:500px; object-fit:contain; background:#f6f8fa; border-radius:6px;" />
</p>

### 3. Tạo bản fork

<p align="center">
  <img src="https://github.com/user-attachments/assets/378721d5-26c5-4898-b523-7e07594f6583" alt="Create fork" style="width:900px; height:500px; object-fit:contain; background:#f6f8fa; border-radius:6px;" />
</p>

> **Note:**
> - Đặt tên repository chính là **tên game** của bạn.
> - Mô tả ngắn gọn về game trong phần **Description**.

Sau khi fork thành công, GitHub sẽ chuyển đến repository mới — đây chính là "bộ khung" để bạn phát triển và hoàn thiện game:

<p align="center">
  <img src="https://github.com/user-attachments/assets/614fafce-3090-4be0-9880-3c7dc7d6f979" alt="Forked repository" style="width:900px; height:500px; object-fit:contain; background:#f6f8fa; border-radius:6px;" />
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
  <img width="1338" height="640" alt="Screenshot from 2026-04-29 15-40-47" src="https://github.com/user-attachments/assets/8922db63-6ff0-4868-8b3a-750f42a10ccb" />
</p>

Bên trong `Workspace`, tạo thêm 2 thư mục con:

| Thư mục   | Mục đích                                                                                       |
| --------- | ---------------------------------------------------------------------------------------------- |
| `Sources` | Chứa các dự án lập trình của bạn                                                               |
| `Tools`   | Chứa các công cụ lập trình (xem chi tiết tại [phần II](#ii-quick-start-guide-setup-môi-trường)) |

<p align="center">
  <img width="1338" height="640" alt="image" src="https://github.com/user-attachments/assets/7bd70fc6-5da7-45b9-ba11-120c7a70cdc2" />
</p>

---

### Bước 2: Clone repo về máy

> **Note:** Bước này chỉ cần thực hiện **một lần duy nhất** khi bắt đầu dự án.

Mở **Terminal** ngay tại thư mục `Sources` và chạy lệnh sau (nhớ thay bằng link repo của bạn):

```bash
git clone https://github.com/<ten-cua-ban>/<ten-repo-da-clone>.git
```

<p align="center">
  <img src="https://github.com/user-attachments/assets/6ba6e15d-ce85-483e-9519-61fcb017dafb" alt="Clone repository" style="width:900px; height:500px; object-fit:contain; background:#f6f8fa; border-radius:6px;" />
</p>

---

### Bước 3: Modify Game

Mở **VSCode** trên Linux, sau đó mở repository vừa clone để bắt đầu lập trình.

Đầu tiên, các bạn hãy đặt tên game của mình trong file Makefile.mk ở thư mục `application/`.

<p align="center">
  <img width="1338" height="640" alt="mkk" src="https://github.com/user-attachments/assets/d0325fb5-6f7c-45a6-bf19-000353f09b85" />
</p>

Toàn bộ logic game được viết trong thư mục `application/`.

<p align="center">
  <img width="1338" height="640" alt="image" src="https://github.com/user-attachments/assets/9bc2a21b-edf5-4a4e-9632-a78a1e738d8c" />
</p>

#### Ví dụ: Hiển thị màn hình Peashooter (Cây đậu bắn súng) trong game Plants vs. Zombies

**Bước 3.1 —** Tạo file header `scr_peashooter.h` trong thư mục `screens/` để khai báo các hàm vẽ màn hình Peashooter:

<p align="center">
  <img width="1338" height="640" alt="image" src="https://github.com/user-attachments/assets/f587a0d8-33f7-45c1-9ec3-5bd804524100" />
</p>

**Bước 3.2 —** Tạo file `scr_peashooter.cpp` để xử lý dữ liệu bitmap và hiển thị Peashooter lên màn hình:

<p align="center">
  <img width="1338" height="640" alt="image" src="https://github.com/user-attachments/assets/b1528e3a-1604-4854-b4a4-acd6b5ea4b14" />
</p>

**Bước 3.3 —** Tạo file header `screens_bitmap.h` trong thư mục `screens/` để khai báo dữ liệu bitmap dùng chung:

<p align="center">
  <img width="1338" height="640" alt="image" src="https://github.com/user-attachments/assets/2796760c-c4f8-40f3-bd9d-00606f21826d" />
</p>

**Bước 3.4 —** Tạo file `screens_bitmap.cpp` chứa dữ liệu bitmap của Peashooter:

<p align="center">
  <img width="1338" height="640" alt="image" src="https://github.com/user-attachments/assets/bc2bf175-f50b-43c5-9dc6-54fb0e3d0288" />
</p>

**Bước 3.5 —** Include file header của Peashooter vào `task_display.h`:

<p align="center">
  <img width="1338" height="640" alt="image" src="https://github.com/user-attachments/assets/9c1bf389-2df6-40cf-a74f-8e8a4267636e" />
</p>

**Bước 3.6 —** Cập nhật lại case `AC_DISPLAY_BUTTON_MODE_RELEASED`:

<p align="center">
  <img width="1338" height="640" alt="Screenshot from 2026-04-29 15-26-56" src="https://github.com/user-attachments/assets/e3a1b7bc-e7fa-4a41-be61-d1095b2b2689" />
</p>

**Bước 3.7 —** Thêm hai file `scr_peashooter.cpp` và `screens_bitmap.cpp` vào `Makefile.mk` trong thư mục `screens/` để biên dịch:

<p align="center">
  <img width="1338" height="640" alt="Screenshot from 2026-04-29 15-28-00" src="https://github.com/user-attachments/assets/09e4b3e7-20b1-48ac-8ede-c25285ea1de6" />
</p>

**Bước 3.8 —** Build và nạp firmware lên kit (xem hướng dẫn chi tiết tại [phần II](#ii-quick-start-guide-setup-môi-trường)):

<p align="center">
  <img src="https://github.com/user-attachments/assets/513e18e3-4141-418a-a290-2bc45f97b54e" alt="Build và nạp firmware" style="width:900px; height:500px; object-fit:contain; background:#f6f8fa; border-radius:6px;" />
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
  <img width="1338" height="640" alt="Screenshot from 2026-04-29 15-36-40" src="https://github.com/user-attachments/assets/5841c5dc-74e6-4714-ab3a-d824be16dc9b" />
</p>

**Repository đã được cập nhật trên GitHub:**

<p align="center">
  <img width="1338" height="640" alt="image" src="https://github.com/user-attachments/assets/0da467a0-5ba5-4058-a1be-20f578c33d36" />
</p>

<p align="center">
  <img width="1338" height="640" alt="image" src="https://github.com/user-attachments/assets/e5e2dc9d-9672-4d4c-b6f5-72a1666ecbfa" />
</p>

Sau bước này, bất kỳ ai cũng có thể truy cập link GitHub của bạn để theo dõi tiến độ và trải nghiệm tựa game mà bạn đã tạo ra.

---

## Liên hệ & Hỗ trợ

- LinkedIn: [www.linkedin.com/in/cao-trong-phuoc](https://www.linkedin.com/in/cao-trong-phuoc)
- Dép lào: 0936310918

## Tài liệu tham khảo

- Blog: [AK Embedded Software](https://epcb.vn/blogs/ak-embedded-software)

---

<p align="center">
  <i>Chúc bạn lập trình vui vẻ và tạo ra những tựa game thật thú vị!</i>
</p>
