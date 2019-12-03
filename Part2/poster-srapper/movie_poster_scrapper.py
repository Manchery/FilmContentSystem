import urllib.request
import re
import os
import sys
import glob
import cv2

def main(html_dir, output_dir):
    file_list = glob.glob(os.path.join(html_dir, '*.html'))
    cnt = 0
    for file in file_list:
        basename = os.path.basename(file)
        filename = os.path.splitext(basename)[0]
        output_path = os.path.join(output_dir, filename+'.jpg')
        html = open(file,'r').read()
        img_url = re.findall(r'src=\".*?\" title=\"点击看更多海报\"', html, re.S)
        img_url = img_url[1][5:-17]
        cap = cv2.VideoCapture(img_url)
        if cap.isOpened():
            ret,img = cap.read()
            cv2.imwrite(output_path,img)
        cnt = cnt+1
        if cnt%10==0:
        	print("%d Done..."%cnt)

if __name__ =='__main__':
    main(sys.argv[1], sys.argv[2])
