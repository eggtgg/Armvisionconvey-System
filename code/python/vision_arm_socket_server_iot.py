from flask import Flask
import socketio
import eventlet
import threading
import cv2
from ultralytics import YOLO

wait_run = []

# Tạo một Socket.IO server
sio = socketio.Server(cors_allowed_origins="*")

# Tạo một Flask app
app = Flask(__name__)

# Kết hợp Flask với Socket.IO
app.wsgi_app = socketio.WSGIApp(sio, app.wsgi_app)

cap = cv2.VideoCapture(0)

model = YOLO('model/best_chai_yolo11.pt')

# Biến toàn cục để quản lý frame cuối cùng
current_frame = None
frame_lock = threading.Lock()


# Hàm hiển thị ảnh cuối cùng trong luồng riêng
def display_last_frame():
    global current_frame
    global wait_run

    while True:
        if current_frame is not None:
            with frame_lock:  # Đảm bảo thread-safe khi truy cập frame
                frame_to_display = current_frame.copy()

            # Hiển thị frame
            cv2.imshow("YOLOv8 Inference", frame_to_display)

        key = cv2.waitKey(30) & 0xFF  # Chờ 30ms
        if key == ord("q"):  # Thoát nếu bấm 'q'
            cv2.destroyAllWindows()
            break

        if key == ord("p"):
            print("an")
            wait_run = []


# Route cơ bản
@app.route('/')
def index():
    return "Socket.IO Server is running!"


# Xử lý sự kiện kết nối
@sio.event
def connect(sid, environ):
    print(f"Client {sid} connected")
    sio.emit('message', {'data': 'Welcome!'}, room=sid)


# Xử lý sự kiện ngắt kết nối
@sio.event
def disconnect(sid):
    print(f"Client {sid} disconnected")


# Xử lý sự kiện custom từ client
@sio.on('from_esp')
def handle_custom_event(sid, data):
    global wait_run, current_frame

    print(f"Received data from {sid}: {data}")

    sensor_name = data.get('sensor')
    sensor_value = data.get('value')

    if cap.isOpened():
        # Đọc một khung hình từ video
        success, frame = cap.read()
        if success:
            if sensor_name == 2 and sensor_value == 0:
                results = model(frame, verbose=False, conf=0.2)
                # Vẽ kết quả lên frame
                annotated_frame = results[0].plot()
                box_bottle = results[0].boxes.xyxy.tolist()

                if box_bottle:
                    xmin, ymin, xmax, ymax = box_bottle[0]
                    length = xmax - xmin
                    width = ymax - ymin
                    print(length, "; ", width, "; ", length / width)
                    raito = length / width
                    if raito > 1:
                        run_mode = {"rmode7": 1, "pick_mode": 0}
                        if ymax > 250:
                            print(ymax)
                            run_mode = {"rmode7": 1, "pick_mode": 2}
                    else:
                        run_mode = {"rmode7": 1, "pick_mode": 1}
                    wait_run.append(run_mode)
                else:
                    run_mode = {"rmode7": 3, "pick_mode": 0}
                    wait_run.append(run_mode)

                # Cập nhật frame cuối cùng
                with frame_lock:
                    current_frame = annotated_frame

            if sensor_name == 1 and sensor_value == 0:
                if wait_run:
                    sio.emit('from_server', wait_run.pop(0))

            if sensor_name == 1 and sensor_value == 1:
                run_mode = {"rmode7": 3, "pick_mode": 0}
                sio.emit('from_server', run_mode)


# Khởi chạy server và luồng hiển thị
if __name__ == '__main__':
    # Tạo luồng hiển thị frame cuối cùng
    display_thread = threading.Thread(target=display_last_frame, daemon=True)
    display_thread.start()

    # Khởi chạy Socket.IO server
    print("Starting server on http://192.168.137.1:3000")
    eventlet.wsgi.server(eventlet.listen(('192.168.137.1', 3000)), app)
