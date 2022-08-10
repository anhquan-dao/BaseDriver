#include "Base.h"

BaseInterface::BaseInterface(uint8_t id_) : id(id_)
{   
    header = new BaseHeader();
    error = new BaseError();
}

BaseInterface::BaseInterface(uint8_t id_, BaseHeader *header_, BaseError *error_) : id(id_)
{
    header = header_;
    error = error_;
}

BaseInterface::~BaseInterface()
{
    if(header!=NULL)
    {
        delete header;
    }
    if(error != NULL)
    {
        delete error;
    }
}

void BaseInterface::writeHeader(uint8_t header_)
{
    writeByte(header->operator()(header_));
}

void BaseInterface::writeError(uint8_t error_)
{
    writeByte(error->operator()(error_));
}

void BaseInterface::writeFloat(float data)
{
    uint8_t float_to_byte_arr[4];
    memcpy(float_to_byte_arr, &data, 4);
    writeByte(float_to_byte_arr, 4);
}

void BaseInterface::writeByte(uint8_t *data, uint8_t size)
{
    for(int i=0; i<size; i++)
    {
#ifdef SERIAL_DEBUG
        Serial.print("0x");
        if(*(data+i)<16) Serial.print("0");
        Serial.print(*(data+i), HEX);
        Serial.print(" ");
#else
        Serial.write(*(data+i));
#endif
    }
}

uint8_t BaseInterface::readByte(uint8_t* buffer, uint8_t size)
{
    uint8_t actual_size = Serial.read(buffer, size);
    return actual_size;

}

uint8_t BaseInterface::readByte(uint8_t* buffer)
{   
    return readByte(buffer, 1);
}

void BaseInterface::readFloat(float *data)
{
    uint8_t buffer[4];
    readByte(buffer, 4);
    data = (float *)buffer;
}

void BaseInterface::readHeader(uint8_t *header)
{
    readByte(header);
}

void BaseInterface::readHeader()
{   
    header_buf[0] = header_buf[1];
    readByte(&header_buf[1]);
}

uint8_t BaseInterface::readMessage(uint8_t *header, uint8_t *buffer)
{
    uint8_t data_len;

    while(*header != 0x43)
    {
        readHeader(header);
    }
    readByte(&data_len);
    readByte(buffer, data_len);
    
    return 0;
}