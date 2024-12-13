# Armvisionconvey-System


# **HỆ THỐNG NHẬN DIỆN VẬT THỂ VÀ ĐIỀU KHIỂN CÁNH TAY ROBOT TỰ ĐỘNG**  
**Khóa luận tốt nghiệp**  
#### **Tác giả:** [Tên của bạn]  
#### **Giảng viên hướng dẫn:** [Tên giảng viên hướng dẫn]  

---

## **Giới thiệu**  
Khóa luận này giới thiệu một hệ thống tích hợp sử dụng **thị giác máy tính** để nhận diện vật thể trên băng chuyền và điều khiển cánh tay robot thực hiện các thao tác tự động như gắp và phân loại vật thể. Hệ thống được xây dựng nhằm mục tiêu ứng dụng trong lĩnh vực tự động hóa, đặc biệt là thu gom và phân loại rác thải thông minh.  

Hệ thống bao gồm các thành phần chính:  
- **Băng chuyền**: Đưa vật thể qua các giai đoạn xử lý.  
- **Cánh tay robot**: Thực hiện thao tác gắp vật thể.  
- **Phần thị giác máy tính**: Nhận diện vật thể và đưa ra quyết định thông qua mô hình **YOLOv8**.  
- **Kết nối không dây IoT**: Đồng bộ các thành phần hệ thống qua giao thức WebSocket.  

---

## **Mục tiêu nghiên cứu**  
1. Phát triển một hệ thống thị giác máy tính để nhận diện vật thể trên băng chuyền.  
2. Tích hợp các thành phần phần cứng và phần mềm, bao gồm cánh tay robot, băng chuyền, và thị giác máy tính.  
3. Đảm bảo khả năng vận hành ổn định, chính xác và hiệu quả trong môi trường thực tế.  

---

## **Tính năng chính của hệ thống**  
- **Nhận diện vật thể thời gian thực**: Mô hình **YOLOv8** được huấn luyện và triển khai để nhận diện vật thể với độ chính xác cao.  
- **Tự động hóa toàn diện**: Cánh tay robot phối hợp nhịp nhàng với băng chuyền thông qua các quyết định từ phần thị giác.  
- **Tăng cường kết nối không dây**: Sử dụng **ESP32** và giao thức **WebSocket** để đồng bộ hóa hệ thống một cách hiệu quả.  
- **Khả năng mở rộng**: Hỗ trợ phát triển nhận diện nhiều loại vật thể và ứng dụng trong các lĩnh vực khác nhau.  

---

## **Cấu trúc hệ thống**  
1. **Phần cứng**:  
   - Arduino Mega và ESP32 để điều khiển băng chuyền và cánh tay robot.  
   - Camera tích hợp để thu thập hình ảnh từ băng chuyền.  
   - Các cảm biến phát hiện vật thể trên băng chuyền.  

2. **Phần mềm**:  
   - **YOLOv8** để nhận diện vật thể.  
   - **Ultralytics Library** để triển khai mô hình trên máy tính cá nhân.  
   - Giao thức **WebSocket** để giao tiếp không dây giữa phần thị giác và hệ thống robot.  

3. **Quy trình hoạt động**:  
   - Băng chuyền đưa vật thể qua vùng quan sát của camera.  
   - Camera thu thập hình ảnh và gửi đến phần thị giác máy tính.  
   - Phần thị giác nhận diện vật thể, gửi quyết định điều khiển qua ESP32 đến Arduino Mega.  
   - Cánh tay robot gắp vật thể theo quyết định từ phần thị giác.  

---

## **Kết quả đạt được**  
- Hệ thống vận hành ổn định, phối hợp nhịp nhàng giữa băng chuyền, cánh tay robot và phần thị giác.  
- Mô hình nhận diện vật thể đạt độ chính xác cao nhờ tập dữ liệu được tối ưu hóa và mô hình **YOLOv8**.  
- Khả năng kết nối không dây đảm bảo sự đồng bộ giữa các thành phần hệ thống.  
- Đáp ứng mục tiêu nghiên cứu, tạo nền tảng cho các ứng dụng tự động hóa thực tế.  

---

## **Hướng phát triển trong tương lai**  
1. **Tối ưu hóa kết nối và vận hành hệ thống** để cải thiện độ chính xác và hiệu suất.  
2. **Mở rộng khả năng nhận diện đa dạng**: Nhận diện nhiều loại vật thể khác nhau, chẳng hạn như chai thủy tinh, lon kim loại.  
3. **Đưa vào ứng dụng thực tiễn**, như tại các cơ sở tái chế, nhà máy sản xuất hoặc dây chuyền tự động hóa công nghiệp.  

---

## **Yêu cầu cài đặt**  
1. Python >= 3.8  
2. Thư viện:  
   - `ultralytics`  
   - `opencv-python`  
   - `flask`  
   - `python-socketio`  
3. Phần cứng:  
   - Arduino Mega, ESP32.  
   - Camera hỗ trợ độ phân giải HD.  

---

## **Hướng dẫn triển khai**  
1. **Cài đặt môi trường**:  
   ```bash
   pip install ultralytics opencv-python flask python-socketio
   ```  

2. **Chạy phần thị giác trên máy tính**:  
   ```bash
   python vision_system.py
   ```  

3. **Nạp chương trình cho Arduino Mega và ESP32**:  
   - Sử dụng **Arduino IDE** để nạp mã điều khiển.  

4. **Kết nối hệ thống**:  
   - Đảm bảo tất cả các thành phần được kết nối qua mạng Wi-Fi cục bộ.  

---

## **Liên hệ**  
- **Tác giả**: [Tên của bạn]  
- **Email**: [Email của bạn]  
- **Github Repository**: [Liên kết đến repo nếu có]  

---
