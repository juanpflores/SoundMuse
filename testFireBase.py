from firebase import firebase
import time
import serial

def writeAndConfirm(ser,msg):
	for i in range(0,20):
		try:
			ser.write(msg.encode())
			m=ser.readline().decode(encoding='UTF-8')
			if m=="Done":
				print("--Hecho")
				return 
		except:
			print("mmmm")
			time.sleep(.1)


def tempToDeg(temp):
	if temp in range(0,20): return "40"
	if temp in range(20,40): return "80"
	if temp in range(40,60): return "120"
	return "160"






myDevice='d001'



firebase = firebase.FirebaseApplication('https://regadera-c338c.firebaseio.com/', None)


ser = serial.Serial(
    port='\\\\.\\COM4',
    baudrate=9600,
    timeout=3
)
if ser.isOpen():
    ser.close()
ser.open()
ser.isOpen()


#constantly check if a user activates the shower
while(True):
	time.sleep(1)
	device= firebase.get('/Devices', myDevice)
	if device['EnUsoPor']!="nadie":# if someone wants to start using the shower
		#turn showerhead on
		writeAndConfirm(ser,"#SC12#")		
		print("Turn showerhead on")
		
		user=firebase.get('/Usuarios',device['EnUsoPor'])
		print(device['EnUsoPor'])
		print(user['Consumo_max'])
		[dTemp,maxWater,casa]=[user['Temperatura_deseada'],user['Consumo_max'],user['Hogar']]
		print ([dTemp,maxWater])
		#change temperature to user's needs
		device['Consumo_max']=maxWater
		device['Temperatura_deseada']=dTemp
		firebase.put('/Devices',myDevice,device)
		time.sleep(10)
		to=time.time()
		consumedWater=0
		while(True):
			device= firebase.get('/Devices', myDevice)
			user=firebase.get('/Usuarios',device['EnUsoPor'])
			if 	device['EnUsoPor']=="nadie":#Si el usuario ya no va a usar la regadera	
				print("Apagando regadera")
				writeAndConfirm(ser,"#AA23#")

				break;	
			if device['Temperatura_deseada']!=dTemp:#Si el usuario cambió la temperatura de la regadera
				dTemp=device['Temperatura_deseada']
				user['Temperatura_deseada']=dTemp
				firebase.put('/Usuarios',device['EnUsoPor'],user)
				writeAndConfirm(ser,"#ST"+tempToDeg(float(dTemp))+"#")
			if device['Consumo_max']!=maxWater:#Si el usuario cambió la temperatura de la regadera
				maxWater=device['Consumo_max']
				user['Consumo_max']=maxWater
				firebase.put('/Usuarios',device['EnUsoPor'],user)

			#pedir el flujo
			try:
				ser.write("#RF#").encode()
				m=ser.readline().decode(encoding='UTF-8')
				cons=int(m)*(time.time()-to)/60
				consumedWater+=cons
				user['Consumo_actual']=int(m)
				user['Consumo_diario']=user['Consumo_diario']+consumedWater
				user['Consumo_mensual']=user['Consumo_mensual']+consumedWater
				firebase.put('/Usuarios',device['EnUsoPor'],user)
			except:
				print("mmmm")
				time.sleep(.1)

				







