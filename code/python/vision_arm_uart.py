import cv2
from ultralytics import YOLO
import serial

arduino_port = 'COM9'  # Replace with the appropriate port
baud_rate = 9600  # Must match the baud rate used in the Arduino sketch
ser = serial.Serial(arduino_port, baud_rate)

wait_run = []

# Load the YOLOv8 model
model = YOLO('best_chai_4.pt')

# Open the video file
cap = cv2.VideoCapture(1)

# Loop through the video frames
while cap.isOpened():
    # Read a frame from the video
    success, frame = cap.read()

    if success:
        # Run YOLOv8 inference on the frame
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').rstrip()
            print(data.split(': '))
            sensor_name, sensor_value = data.split(': ')
            if sensor_name == 'val2' and sensor_value == '0':
                results = model(frame, verbose=False, conf=0.87)

                # Visualize the results on the frame
                annotated_frame = results[0].plot()
                # Display the annotated frame
                cv2.imshow("YOLOv8 Inference", annotated_frame)
                box_bottle = results[0].boxes.xyxy.tolist()
                if box_bottle:
                    xmin, ymin, xmax, ymax = box_bottle[0]
                    length = xmax - xmin
                    width = ymax - ymin
                    print(length, "; ", width, "; ", length / width)
                    raito = length / width
                    if raito > 1:
                        run_mode = '{"rmode7":1,"pick_mode":0}'
                        if ymax > 250:
                            print(ymax)
                            run_mode = '{"rmode7":1,"pick_mode":2}'
                    else:
                        run_mode = '{"rmode7":1,"pick_mode":1}'
                    wait_run.append(run_mode)
                    #ser.write(run_mode.encode())
                else:
                    run_mode = '{"rmode7":3,"pick_mode":0}'
                    wait_run.append(run_mode)
                    #ser.write(run_mode.encode())
            if sensor_name == 'val1' and sensor_value == '0':
                if(wait_run):
                    ser.write(wait_run.pop(0).encode())

            if sensor_name == 'val1' and sensor_value == '1':
                run_mode = '{"rmode7":3,"pick_mode":0}'
                ser.write(run_mode.encode())

        # Break the loop if 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break
        if cv2.waitKey(1) & 0xFF == ord("p"):
            print("an")
            wait_run = []
    else:
        # Break the loop if the end of the video is reached
        break

# Release the video capture object and close the display window
cap.release()
cv2.destroyAllWindows()
