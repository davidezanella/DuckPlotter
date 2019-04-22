# DuckPlotter

## Hot to use DuckPlotter

### Configure the system

* Create the physical structure of the DuckPlotter
* Send code to Arduino
* Install all python requirements:

    go into computer_code folder and run 
    
    `pip install -r requirements.txt`

### Generate the Gcode file from an image

* Download and install the [Inkscape](https://inkscape.org) program and its [Laser Tool Plug-In](http://jtechphotonics.com/?page_id=2012)
* Open your image with Inkscape
* Convert the image to a Path image using the grey scale option
* Ungroup the layers of the image
* Choose the best layer and fix it removing or adding pixels

    If you need help doing these steps follow [this video](https://www.youtube.com/watch?v=U248SJcxRWA).

* Finally export Gcode from your image using the "Extensions" tab and selecting the option "Generate Gcode"

### Start the plotting process

* Execute the duckPlotter program:

    `python duckPlotter.py [gcode-file] [serial-port]`

## Used libraries:
* [ArduinoJson](https://arduinojson.org)