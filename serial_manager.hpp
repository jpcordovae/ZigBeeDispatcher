/*
 * File:   serial_manager.hpp
 * Author: Juan Pablo Cordova E.
 *
 * Created on May 5, 2012, 10:58 AM
 *
 *
 */

#ifndef SERIAL_MANAGER_HPP
#define SERIAL_MANAGER_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>
#include <boost/any.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <iostream>
#include <vector>
#include <queue>

class Serial {
public:

    typedef std::vector<boost::uint8_t> buffer_type; // buffer of bytes, contiguous memory
    typedef boost::shared_ptr<buffer_type> buffer_type_ptr;
    typedef boost::signals2::signal<void ( boost::posix_time::ptime, buffer_type_ptr) > OnNewDataSignal;

    // define constants for univertal or local time timestamps
    enum serial_port_timestamp { UNIVERSAL_TIME_TIMESTAMP, LOCAL_TIME_TIMESTAMP};

    Serial(const char *dev_name) : m_io(), m_port(m_io, dev_name)
        {
            read_some();
            // run the IO service as a separate thread, so the main thread can do others
            boost::thread t(boost::bind(&boost::asio::io_service::run, &m_io));
            read_buffer = buffer_type_ptr(new buffer_type);
            m_dev_name = dev_name;
        }

    ~Serial(void){
        m_port.close();
    }

    // open or reopen device
    void open(const char *devname,
              unsigned int baud_rate,
              boost::asio::serial_port::parity opt_parity = boost::asio::serial_port::parity(boost::asio::serial_port::parity::none),
              boost::asio::serial_port::character_size opt_csize = boost::asio::serial_port::character_size(8),
              boost::asio::serial_port::flow_control opt_flow = boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none),
              boost::asio::serial_port::stop_bits opt_stop = boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one)) {

        m_port.cancel();

        if (m_port.is_open()) {
            m_port.close();
        }

        m_port.open(devname);
        m_port.set_option(boost::asio::serial_port_base::parity(opt_parity));
        m_port.set_option(boost::asio::serial_port::character_size(opt_csize));
        m_port.set_option(boost::asio::serial_port::flow_control(opt_flow));
        m_port.set_option(boost::asio::serial_port::stop_bits(opt_stop));

        if (!m_port.is_open()) {
            std::cerr << "fail to open serial port " << std::string(devname);
        }
    }

    boost::signals2::connection ConnectCallBack(const OnNewDataSignal::slot_type &_subscriber) {
        return OnNewData.connect(_subscriber);
    }

    void close(void){
        m_port.close();
    }

    bool isOpen(void){
        return m_port.is_open();
    }

    std::string getDevName(void){
        return m_dev_name;
    }

    void setUniversalTime(){
        m_serial_port_timestamp = UNIVERSAL_TIME_TIMESTAMP;
    }

    void setLocalTime(){
        m_serial_port_timestamp = LOCAL_TIME_TIMESTAMP;
    }

private:
    enum serial_port_timestamp m_serial_port_timestamp;
    OnNewDataSignal OnNewData;

    bool m_bNewData; // new data in buffer data
    boost::mutex m_message_queue_mtx;
    std::queue<buffer_type_ptr> messages_queue; // for polling reading
    buffer_type_ptr read_buffer;
    char read_msg_[512];
    boost::asio::io_service m_io;
    boost::asio::serial_port m_port;
    std::string m_dev_name;

    //boost::asio::serial_port::baud_rate _m_baud_rate;
    //boost::asio::serial_port::character_size _m_character_set;
    //boost::asio::serial_port::flow_control _m_flow_control;
    //boost::asio::serial_port::parity _m_parity;
    //boost::asio::serial_port::stop_bits _m_stop_bits;

    void readHandler(const boost::system::error_code& error, size_t bytes_transferred) {
        read_msg_[bytes_transferred] = 0;
        for (size_t i = 0; i < bytes_transferred; i++) {

            read_buffer->push_back(read_msg_[i]);
            m_bNewData = true;

            if (read_msg_[i] == '\n')
	    {
                messages_queue.push(read_buffer); // save buffer to polling
						  // queue
		
                if(m_serial_port_timestamp==UNIVERSAL_TIME_TIMESTAMP)
		{
                    OnNewData(boost::posix_time::microsec_clock::universal_time(),read_buffer);
                }else{
                    OnNewData(boost::posix_time::microsec_clock::local_time(),read_buffer);
                }
		
                read_buffer = buffer_type_ptr(new buffer_type);
            }

        }
        read_some();
    }

    void read_some() {
        m_port.async_read_some(boost::asio::buffer(read_msg_, 512),
                               boost::bind(&Serial::readHandler,
                                           this,
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
    }

    void async_write(const char *_buffer, size_t _buffer_size) // write to serial port
        {
            m_port.async_write_some(boost::asio::buffer(_buffer, _buffer_size),
                                    boost::bind(&Serial::write_handler,
                                                this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred)
                );
        }

    void write_handler(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (error) {
            return;
        }
    }

};

#endif  /* SERIAL_MANAGER_HPP */

