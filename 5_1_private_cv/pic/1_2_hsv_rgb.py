import cv2
ax=0
ay=0
name='p1.bmp'
img =cv2.imread(name)
hsv=cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
def mouse_event(event, x,y,flags,param):
	if event==cv2.EVENT_LBUTTONDOWN:
		img =cv2.imread(name)
		print('PIX:',x,y)
		print('BGR:',img[y,x])
		print('HSV:',hsv[y,x])
		global ax
		global ay
		ax=x
		ay=y
		cv2.circle(img,(x,y),3,(255,255,0),1)#draw circle
		cv2.imshow('press_q_to_exit',img)
		
cv2.namedWindow("press_q_to_exit")		
cv2.setMouseCallback("press_q_to_exit",mouse_event)		
if __name__=="__main__":
	cv2.imshow('press_q_to_exit',img)
	while (cv2.waitKey(1) & 0xFF) != ord('q'):
		cv2.setMouseCallback("press_q_to_exit",mouse_event)
	cv2.destroyAllWindows()
