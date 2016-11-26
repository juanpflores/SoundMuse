
import time
import serial



ser = serial.Serial(
    port='\\\\.\\COM4',
    baudrate=9600,
    timeout=3
)


if ser.isOpen():
    ser.close()
ser.open()
ser.isOpen()
time.sleep(1)


while True:
	msg=input('write a Message ')
	if ser.isOpen():
		#Write
		ser.write(msg.encode())
		out = ''
		timeout = time.time() + 10
		print("message sent")
		timeout = time.time() + 10
		print (ser.readline().decode(encoding='UTF-8'))
		"""r=False
								w=False
								while True:
									try:
										#while ser.ava
										t=ser.read().decode("utf-8")
										if w and t!="#":
											out+=t
										if w and t=="#":
											break;
										if not w and t=="#":
											w=True
						
										print((t))
										
									except:
										print("faiiil")
								print ("Hey")
								print (out)		"""

		# let's wait one second before reading output (let's give device time to answer)
	time.sleep(5)



ser.close()

input("Done")