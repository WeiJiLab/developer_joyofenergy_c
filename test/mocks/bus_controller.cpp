#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "meter.h"
#include "bus_controller.h"

#define BC_MSG_CNT 0x10

class bus_controller {
  public:
    bus_controller() {
        task = std::thread(&bus_controller::bus_dispatch_to_slave_entity, this);
        task.detach();
    }

    ~bus_controller() {
    }

    static bus_controller *get_instance() {
        static bus_controller bc;
        return &bc;
    }

    int master_write_register(struct message *msg) {
        std::unique_lock<std::mutex> sem(m2smtx);
        memcpy(&m2s_dma_buff[0], msg, sizeof(struct message));
        m2s_msg_cnt = 1;
        m2scv.notify_all();
        return 0;
    }

    int master_read_register(struct message *msg) {
        std::unique_lock<std::mutex> sem(s2mmtx);
        while (0 == s2m_msg_cnt) {
            s2mcv.wait(sem);
        }
        memcpy(msg, &s2m_dma_buff[0], sizeof(struct message));
        s2m_msg_cnt = 0;
        return 0;
    }

    int slave_write_register(struct message *msg) {
        std::unique_lock<std::mutex> sem(s2mmtx);
        memcpy(&s2m_dma_buff[0], msg, sizeof(struct message));
        s2m_msg_cnt = 1;
        s2mcv.notify_all();
        return 0;
    }

    // called from thread
    int slave_read_register(struct message *msg) {
#if 0
        std::unique_lock<std::mutex> sem(m2smtx);
        while (0 == m2s_msg_cnt) {
            m2scv.wait(sem);
        }
#endif
        if (0 == m2s_msg_cnt) {
            return -1;
        }
        memcpy(msg, &m2s_dma_buff[0], sizeof(struct message));
        m2s_msg_cnt = 0;
        return 0;
    }

  private:
    void bus_dispatch_to_slave_entity(void) {
        uint32_t i;

        std::cout << "bc task start..." << std::endl;
        while (1) {
            std::unique_lock<std::mutex> sem(m2smtx);
            /* 1. producer & consumer sequence
               2. spurious wakeups  */
            while (0 == m2s_msg_cnt) {
                m2scv.wait(sem);
            }

            for (i = 0; (i < m2s_msg_cnt) && (i < BC_MSG_CNT); i++) {
                device_interrupt_proc(m2s_dma_buff[i].head.meter_id);
            }
        }
        std::cout << "bc task terminated..." << std::endl;
    }

  private:
    std::thread task;
    std::mutex s2mmtx;
    std::mutex m2smtx;
    std::condition_variable m2scv;
    std::condition_variable s2mcv;
    struct message m2s_dma_buff[BC_MSG_CNT];
    size_t m2s_msg_cnt = 0;
    struct message s2m_dma_buff[BC_MSG_CNT];
    size_t s2m_msg_cnt = 0;
};

int master_receive_message(struct message *msg)
{
    if (msg) {
        return bus_controller::get_instance()->master_read_register(msg);
    }
    return -1;
}

int master_send_message(struct message *msg)
{
    if (msg) {
        return bus_controller::get_instance()->master_write_register(msg);
    }
    return -1;
}

int slave_send_message(struct message *msg)
{
    if (msg) {
        return bus_controller::get_instance()->slave_write_register(msg);
    }
    return -1;
}

int slave_receive_message(struct message *msg)
{
    if (msg) {
        return bus_controller::get_instance()->slave_read_register(msg);
    }
    return -1;
}
