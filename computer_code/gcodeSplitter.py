#!/usr/bin/python3
import sys
import math
import os


def main():
    n_lines = 2500  

    if len(sys.argv) > 1:
        gcode_file_name = sys.argv[1]
    else:
        gcode_file_name = input("Name of the gcode file to split: ")
    if len(sys.argv) > 2:
        n_lines = int(sys.argv[2])

    gcode_file = open(gcode_file_name, 'r')
    gcode = gcode_file.readlines()

    n_files = math.ceil(len(gcode) / n_lines)
    base_name = os.path.splitext(gcode_file_name)[0]

    for i in range(n_files):
        file_code = gcode[i * n_lines:(i+1) * n_lines]
        file_code = ['M03\n'] + file_code + ['M05\n'] #move down the pen at the start and down at the end

        file_name = base_name + "_" + (str(i+1)).zfill(4) + ".gcode"

        with open(file_name, 'w') as f:
            f.writelines(file_code)


    print(str(n_files) + " gcode files generated.")
    print("Execute the first one and then the others with the '-c' option without moving or resetting the plotter.")


if __name__ == "__main__":
    main()
