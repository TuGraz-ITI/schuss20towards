### About

This repository supplements the paper "Towards an Automated Monitoring of RF Activity in Low-Power Wireless Testbeds" published in the MobiCom 2020 Workshop CPS-IoTBench. It contains the scripts used to create all plots, as well as the firmware and scripts to obtain all data.

### Folder Structure
 * **contiki**: Source code for TelosB low-power  nodes used for the unicast experiment.
 * **pattern**: The GPIO traces recorded with D-Cube and exported as csv.
 * **firmware**: The modified firmware for the BCM43455C0 radio.
 * **scripts**: Reader script used to record all power estimates to files.
 * **rpi90**-**rpi94**: power estimates used to generate the Figures for the paper.

## Captures
The figures were created from power estimates captured by a modified version of the BCM43455C0 firmware and depends on a modified kernel module from the nexmon project. After loading the capture can be started by sending a ioctl 900 to the radio. The radio will now send UDP packets to port 5500, these can be logged to the used format using script/reader.py.

## Figures
All plots are created using jupyter notebooks and rely on matplotlib, pandas and numpyi. The captions are created using latex and Figure 6 additionally depends on pyopencv.

## Contiki
To create Figure 7b two TelosB sky nodes were used. The PRR was logged using D-Cube's GPIO traces.
