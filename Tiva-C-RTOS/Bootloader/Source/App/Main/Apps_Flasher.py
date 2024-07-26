import tkinter as tk
from tkinter import messagebox, filedialog
import serial.tools.list_ports
import os
import serial
import time
import tkinter.ttk as ttk

class Form1:
    def __init__(self, root):
        self.root = root
        self.root.title("Serial Port Application")

        self.label1 = tk.Label(root, text="File Path:")
        self.label1.grid(row=0, column=0)

        self.txtFilePath = tk.Entry(root, width=50)
        self.txtFilePath.grid(row=0, column=1, columnspan=3)

        self.btnSelectFile = tk.Button(root, text="Select File", command=self.select_file, bg='lightblue', fg='black')
        self.btnSelectFile.grid(row=0, column=4)

        self.label2 = tk.Label(root, text="COM Port:")
        self.label2.grid(row=1, column=0)

        self.cmbComPorts = tk.StringVar(root)
        self.cmbComPorts.set("Select COM Port")
        self.com_ports_menu = tk.OptionMenu(root, self.cmbComPorts, *self.get_com_ports())
        self.com_ports_menu.grid(row=1, column=1)

        self.label3 = tk.Label(root, text="Baud Rate:")
        self.label3.grid(row=2, column=0)

        baud_rates = [9600, 115200, 230400]
        self.txtBaudRate = tk.StringVar(root)
        self.txtBaudRate.set("115200")
        self.baud_rate_menu = tk.OptionMenu(root, self.txtBaudRate, *baud_rates)
        self.baud_rate_menu.grid(row=2, column=1)

        self.label4 = tk.Label(root, text="Data Packet Size:")
        self.label4.grid(row=3, column=0)

        packet_sizes = ["512b", "1KB", "2KB", "4KB"]
        self.txtPacketSize = tk.StringVar(root)
        self.txtPacketSize.set("1KB")
        self.packet_size_menu = tk.OptionMenu(root, self.txtPacketSize, *packet_sizes)
        self.packet_size_menu.grid(row=3, column=1)

        self.btnSend = tk.Button(root, text="Send", command=self.send_data)
        self.btnSend.grid(row=3, column=4)

        self.logBox = tk.Text(root, height=10, width=60)
        self.logBox.grid(row=4, column=0, columnspan=5)

        self.scrollbar = tk.Scrollbar(root, command=self.logBox.yview)
        self.scrollbar.grid(row=4, column=5, sticky='ns')
        self.logBox.config(yscrollcommand=self.scrollbar.set)

        self.progressBar = ttk.Progressbar(root, orient="horizontal", length=400, mode="determinate")
        self.progressBar.grid(row=5, column=0, columnspan=5)

        self.statusText = tk.Text(root, height=5, width=60)
        self.statusText.grid(row=6, column=0, columnspan=5)

    def get_com_ports(self):
        ports = [port.device for port in serial.tools.list_ports.comports()]
        if not ports:
            return ["COMM_PORT_UNAVAILABLE"]
        return ports

    def select_file(self):
        file_path = filedialog.askopenfilename(
            filetypes = [("Binary Files", "*.bin")])
        if file_path:
            self.txtFilePath.delete(0, tk.END)
            self.txtFilePath.insert(0, file_path)

    def send_data(self):
        file_path = self.txtFilePath.get()
        com_port = self.cmbComPorts.get()
        baud_rate = int(self.txtBaudRate.get())

        if com_port == "Select COM Port":
            self.log_message("ERROR: Please select a COM port.")
            return

        if not os.path.isfile(file_path):
            self.log_message("ERROR: File does not exist.")
            return

        try:
            # Open the serial port
            ser = serial.Serial(com_port, baud_rate, timeout=1)

            # Handshake: Send test byte 0xAA
            ser.write(bytes([0xAA]))

            # Wait for an ACK back after sending the hand shaking data
            if not self.wait_for_ack(ser, "ERROR: Hand-shake failure with bootloader"):
                return

            # Get the size of the file
            file_size = os.path.getsize(file_path)
            

            # Send the file size as the first 4 bytes
            ser.write(file_size.to_bytes(4, byteorder='little'))

            # Wait for an ACK back after sending the size of the file
            if not self.wait_for_ack(ser, "ERROR: ACK not received after sending file size"):
                return


            #number of bytes sending at a time
            NUM_OF_BYTE_SENDING = 4
            
            # Continue with file transfer
            packet_size        = self.get_packet_size()
            file_size_Kb       = int(file_size/1024)
            packet_num         = int(file_size / packet_size)
            extra_byte         = int(file_size % packet_size)
            file_size_aling    = file_size - (file_size % NUM_OF_BYTE_SENDING)
            progress_increment = int(0)
            
            
            
            self.log_message("INFO: File size in Kb   = " + str(file_size_Kb))
            self.log_message("INFO: Packet_size       = " + str(packet_size))
            self.log_message("INFO: Total packets     = " + str(packet_num))
            self.log_message("INFO: Extra Bytes       = " + str(extra_byte))
            self.log_message("INFO: Sending data      = " + str(0) + "%")



             # Send the packet size as the 2nd 4 bytes
            ser.write(packet_size.to_bytes(4, byteorder='little'))

            # Wait for an ACK back after sending the size of the file
            if not self.wait_for_ack(ser, "ERROR: ACK not received after sending packet size"):
                return
                
                
            with open(file_path, 'rb') as file:
                total_bytes_sent = 0

                

                #sending the alinged bytes
                for i in range(0, file_size_aling, NUM_OF_BYTE_SENDING):
                    progress_increment += NUM_OF_BYTE_SENDING
                    if ((i != 0) and ((i % packet_size) == 0)):
                        if not self.wait_for_ack(ser, "ERROR: ACK not received after sending data packet"):
                            return

                    data = file.read(NUM_OF_BYTE_SENDING)
                    ser.write(data)

                    progress_percentage = (progress_increment / file_size) * 100
                    self.update_progress(progress_percentage)

                    self.logBox.delete('end - 2 lines', 'end - 1 lines')
                    self.log_message("INFO: Sending data = " + str(progress_percentage) +"%")
                    
                #sending the odd bytes apart from alinged bytes
                for i in range(0, (file_size % NUM_OF_BYTE_SENDING), 1):
                    progress_increment += 1
                    data = file.read(1)
                    ser.write(data)
                    progress_percentage = (progress_increment / file_size) * 100
                    self.update_progress(progress_percentage)
            
                    self.logBox.delete('end - 2 lines', 'end - 1 lines')
                    self.log_message("INFO: Sending data = " + str(progress_percentage) +"%")

            #printing the last message
            self.logBox.delete('end - 2 lines', 'end - 1 lines')
            self.log_message("INFO: Sending data = " + str(100) +"%")

            # Close the serial port
            ser.close()

            self.log_message("INFO: File sent successfully.")

        except serial.SerialException as e:
            self.log_message(f"Serial port error: {e}")
        except Exception as e:
            self.log_message(f"An error occurred: {e}")

    def get_packet_size(self):
        size_str = self.txtPacketSize.get()
        if size_str == "512b":
            return 512
        elif size_str == "1KB":
            return 1024
        elif size_str == "2KB":
            return 2048
        elif size_str == "4KB":
            return 4096
        else:
            return 1024  # Default to 1KB

    def update_progress(self, percentage):
        self.progressBar["value"] = percentage
        self.root.update_idletasks()

    def log_message(self, message):
        self.logBox.insert(tk.END, message + '\n')
        self.logBox.see(tk.END)
        
    def wait_for_ack(self, ser, error_message):
        """
        Wait for an ACK byte after sending data.

        Parameters:
        - ser: Serial port object
        - error_message: Message to display if ACK is not received

        Returns:
        - True if ACK is received within 2 seconds, False otherwise
        """
        start_time = time.time()
        confirmation_byte = None
        while time.time() - start_time < 2:
            confirmation_byte = ser.read(1)
            if confirmation_byte == bytes([0x55]):
                return True
        else:
            self.log_message(error_message)
            ser.close()
            return False
        

def main():
    root = tk.Tk()
    app = Form1(root)
    root.mainloop()

if __name__ == "__main__":
    main()
