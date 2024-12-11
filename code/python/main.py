import threading
import eventlet
import vision_arm_socket_server_iot


def itandai_family():
    print("May the Force be with us! Always!")


if __name__ == '__main__':

    # Tạo luồng hiển thị frame cuối cùng
    display_thread = threading.Thread(target=vision_arm_socket_server_iot.display_last_frame, daemon=True)
    display_thread.start()

    # Khởi chạy Socket.IO server
    print("Starting server on http://192.168.137.1:3000")
    eventlet.wsgi.server(eventlet.listen(('192.168.137.1', 3000)), vision_arm_socket_server_iot.app)
    itandai_family()
