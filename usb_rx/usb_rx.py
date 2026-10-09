import serial
import ctypes
import struct
import numpy as np
from threading import Thread
import crc16
from openpyxl import Workbook
import os

class serial_node(serial.Serial):
    def __init__(self):
        super().__init__('COM5',115200)
        self.crc=crc16.crc16()

        self.s=struct.Struct('=B4fH')
        self.rx_buffer=ctypes.create_string_buffer(self.s.size)
        self.rx_data=np.zeros(1,dtype=self.rx_bag)

        self.wb=Workbook()
        self.ws=self.wb.active
        self.ws.title = "Sheet1"
        self.ws.append(['r','g','b','w'])

        self.excel_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "data.xlsx")
        self.cnt=0

    rx_bag=np.dtype({
        'names':['header','intensity','crc_sum'],
        'formats':['B','4f','H']
    })

    def serial_open(self):
        while True:
            if self.is_open:
                break
            else:
                try:
                    self.open()
                    print("Serial open success.")
                    break
                except serial.SerialException:
                    None

    def serial_restart(self):
        try:
            self.close()
            self.open()
            print("Serial reset success.")
        except:
            None

    def receive(self):
        while True:
            try:
                if self.inWaiting()==self.s.size:
                    self.rx_buffer=self.read(self.s.size)
                    if self.rx_buffer[0]==0x5A and self.crc.verify(self.rx_buffer,self.s.size):
                        self.unpacked_data = self.s.unpack_from(self.rx_buffer,0)
                        self.rx_data['header']=self.unpacked_data[0]
                        self.rx_data['intensity']=[
                            float(self.unpacked_data[1]),
                            float(self.unpacked_data[2]),
                            float(self.unpacked_data[3]),
                            float(self.unpacked_data[4])
                        ]
                        self.rx_data['crc_sum']=self.unpacked_data[5]
                        self.ws.append((list)(np.hstack((self.rx_data['intensity'][0]))))
                        self.wb.save(self.excel_path)
                        self.cnt+=1
                        print(f" 📥 收到 {self.cnt} 组数据",end="\r",flush=True)

                else:
                    None
            except serial.SerialException:
                None
            except:
                self.serial_restart()

def main():
    node=serial_node()
    node.serial_open()

    executor_thread = Thread(target=node.receive, daemon=True)
    executor_thread.start()

    print("程序已启动，退出请按Ctrl+C...")
    try:
        executor_thread.join()
    except KeyboardInterrupt:
        print(f"\n正在退出程序，数据已保存到{node.excel_path}。")

if __name__ == "__main__":
 	main()
