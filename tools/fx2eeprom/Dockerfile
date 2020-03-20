FROM debian:stable

RUN apt-get update
RUN apt-get -y install libusb-1.0-0-dev gcc make
ADD . /fx2eeprom
WORKDIR /fx2eeprom
RUN make
