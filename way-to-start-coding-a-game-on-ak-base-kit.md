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
  - [Bước 5: Cập nhật source code trước mỗi phiên làm việc](#bước-5-cập-nhật-source-code-trước-mỗi-phiên-làm-việc)

---

## I. Tạo "Sân chơi riêng" (Fork)

Để khởi tạo dự án cá nhân, bạn thực hiện theo các bước sau:

### 1. Truy cập repository gốc

**Link:** [https://github.com/the-ak-foundation/ak-base-kit-stm32l151](https://github.com/the-ak-foundation/ak-base-kit-stm32l151)

### 2. Fork repository

Nhấn nút **Fork** ở góc trên bên phải để tạo một bản sao dự án về tài khoản cá nhân của bạn.
Ngoài ra, bạn có thể nhấn nút **Star** ở bên phải nút **Fork** để ủng hộ tác giả nhé.

<p align="center">
  <img width="1338" height="640" alt="Fork repository" src="https://github.com/user-attachments/assets/eb87b69c-5c76-40b2-bc36-869757adf1a1" />
</p>

### 3. Tạo bản fork

<p align="center">
  <img width="1338" height="640" alt="Create fork" src="https://github.com/user-attachments/assets/b96d21fa-53be-4dd6-93d6-cea6b0efdad9" />
</p>

> **Note:**
> - Đặt tên repository chính là **tên game** của bạn.
> - Mô tả ngắn gọn về game trong phần **Description**.

Sau khi fork thành công, GitHub sẽ chuyển đến repository mới — đây chính là "bộ khung" để bạn phát triển và hoàn thiện game:

<p align="center">
  <img width="1338" height="640" alt="Forked repository" src="https://github.com/user-attachments/assets/614fafce-3090-4be0-9880-3c7dc7d6f979" />
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
  <img width="1338" height="640" alt="Tạo thư mục Workspace" src="https://github.com/user-attachments/assets/8922db63-6ff0-4868-8b3a-750f42a10ccb" />
</p>

Bên trong `Workspace`, tạo thêm 2 thư mục con:

| Thư mục   | Mục đích                                                                                       |
| --------- | ---------------------------------------------------------------------------------------------- |
| `Sources` | Chứa các dự án lập trình của bạn                                                               |
| `Tools`   | Chứa các công cụ lập trình (xem chi tiết tại [phần II](#ii-quick-start-guide-setup-môi-trường)) |

<p align="center">
  <img width="1338" height="640" alt="Cấu trúc Workspace" src="https://github.com/user-attachments/assets/7bd70fc6-5da7-45b9-ba11-120c7a70cdc2" />
</p>

---

### Bước 2: Clone repo về máy

> **Note:** Bước này chỉ cần thực hiện **một lần duy nhất** khi bắt đầu dự án.

Mở **Terminal** ngay tại thư mục `Sources` và chạy lệnh sau (nhớ thay bằng link repo của bạn):

```bash
git clone https://github.com/<ten-cua-ban>/<ten-repo-da-clone>.git
```

<p align="center">
  <img width="1338" height="640" alt="Clone repository" src="https://github.com/user-attachments/assets/c58bd5e7-b73e-4095-b3c6-484a43f09ec8" />
</p>

---

### Bước 3: Modify Game

Mở **VSCode** trên Linux, sau đó mở repository vừa clone để bắt đầu lập trình.

Trước tiên, hãy đặt tên cho game của bạn trong file `Makefile.mk` ở thư mục `application/`:

<p align="center">
  <img width="1338" height="640" alt="Đặt tên game trong Makefile.mk" src="https://github.com/user-attachments/assets/d0325fb5-6f7c-45a6-bf19-000353f09b85" />
</p>

Toàn bộ logic game được viết trong thư mục `application/`.

<p align="center">
  <img width="1338" height="640" alt="Cấu trúc thư mục application" src="https://github.com/user-attachments/assets/9bc2a21b-edf5-4a4e-9632-a78a1e738d8c" />
</p>

#### Ví dụ: Hiển thị màn hình Peashooter (Cây đậu bắn súng) trong game Plants vs. Zombies

**Bước 3.1 —** Tạo file header `scr_peashooter.h` trong thư mục `screens/` để khai báo các hàm vẽ màn hình Peashooter:

<p align="center">
  <img width="1338" height="640" alt="scr_peashooter.h" src="https://github.com/user-attachments/assets/f587a0d8-33f7-45c1-9ec3-5bd804524100" />
</p>

**Bước 3.2 —** Tạo file `scr_peashooter.cpp` để xử lý dữ liệu bitmap và hiển thị Peashooter lên màn hình:

<p align="center">
  <img width="1338" height="640" alt="scr_peashooter.cpp" src="https://github.com/user-attachments/assets/b1528e3a-1604-4854-b4a4-acd6b5ea4b14" />
</p>

**Bước 3.3 —** Tạo file header `screens_bitmap.h` trong thư mục `screens/` để khai báo dữ liệu bitmap dùng chung:

<p align="center">
  <img width="1338" height="640" alt="screens_bitmap.h" src="https://github.com/user-attachments/assets/2796760c-c4f8-40f3-bd9d-00606f21826d" />
</p>

**Bước 3.4 —** Tạo file `screens_bitmap.cpp` chứa dữ liệu bitmap của Peashooter:

<p align="center">
  <img width="1338" height="640" alt="screens_bitmap.cpp" src="https://github.com/user-attachments/assets/bc2bf175-f50b-43c5-9dc6-54fb0e3d0288" />
</p>

**Bước 3.5 —** Include file header của Peashooter vào `task_display.h`:

<p align="center">
  <img width="1338" height="640" alt="task_display.h" src="https://github.com/user-attachments/assets/9c1bf389-2df6-40cf-a74f-8e8a4267636e" />
</p>

**Bước 3.6 —** Cập nhật lại case `AC_DISPLAY_BUTTON_MODE_RELEASED`:

<p align="center">
  <img width="1338" height="640" alt="AC_DISPLAY_BUTTON_MODE_RELEASED" src="https://github.com/user-attachments/assets/e3a1b7bc-e7fa-4a41-be61-d1095b2b2689" />
</p>

**Bước 3.7 —** Thêm hai file `scr_peashooter.cpp` và `screens_bitmap.cpp` vào `Makefile.mk` trong thư mục `screens/` để biên dịch:

<p align="center">
  <img width="1338" height="640" alt="Makefile.mk" src="https://github.com/user-attachments/assets/09e4b3e7-20b1-48ac-8ede-c25285ea1de6" />
</p>

**Bước 3.8 —** Build và nạp firmware lên kit (xem hướng dẫn chi tiết tại [phần II](#ii-quick-start-guide-setup-môi-trường)):

<p align="center">
  <img width="1338" height="640" alt="Build và nạp firmware" src="https://github.com/user-attachments/assets/f3d4ab4c-7437-4ef5-8cca-6fc61ea7c2c6" />
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
  <img width="1338" height="640" alt="Kết quả git push" src="https://github.com/user-attachments/assets/5841c5dc-74e6-4714-ab3a-d824be16dc9b" />
</p>

**Repository đã được cập nhật trên GitHub:**

<p align="center">
  <img width="1338" height="640" alt="Repository đã cập nhật" src="https://github.com/user-attachments/assets/0da467a0-5ba5-4058-a1be-20f578c33d36" />
</p>

<p align="center">
  <img width="1338" height="640" alt="Chi tiết repository" src="https://github.com/user-attachments/assets/e5e2dc9d-9672-4d4c-b6f5-72a1666ecbfa" />
</p>

Từ đây, bất kỳ ai cũng có thể truy cập link GitHub của bạn để theo dõi tiến độ và trải nghiệm tựa game mà bạn đã tạo ra.

---

### Bước 5: Cập nhật source code trước mỗi phiên làm việc

Trước mỗi lần ngồi vào "code", hãy đồng bộ source code mới nhất từ GitHub về máy bằng lệnh sau (chạy tại **thư mục gốc** của repo):

```bash
git pull origin main --rebase
```

> **Note:** Lệnh này giúp bạn lấy về toàn bộ thay đổi mới nhất từ GitHub, đồng thời sắp xếp lại các commit của bạn một cách gọn gàng — tránh tình trạng xung đột (conflict) hay lịch sử git rối rắm. Sau khi đồng bộ xong, bạn cứ thoải mái lập trình; đến khi muốn lưu tiến độ, chỉ việc dùng lại 3 lệnh `add` → `commit` → `push` ở [Bước 4](#bước-4-push-code-lên-github) là xong.

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
