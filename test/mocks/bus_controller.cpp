#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "meter.h"
#include "bus_controller.h"
#include "bus_message_queue.h"

#define BC_MSG_CNT 0x10

class bus_controller {
  public:
    bus_controller() {
        m2s_queue = create_bus_message_queue(BC_MSG_CNT);
        assert(NULL != m2s_queue);
        s2m_queue = create_bus_message_queue(BC_MSG_CNT);
        assert(NULL != s2m_queue);
        reg_start = true;
        task = std::thread(&bus_controller::bus_dispatch_to_slave_entity, this);
        task.detach();
    }

    ~bus_controller() {
        destory_bus_message_queue(m2s_queue);
        destory_bus_message_queue(s2m_queue);
        thread_cancel();
    }

    static bus_controller *get_instance() {
        static bus_controller bc;
        return &bc;
    }

    int master_write_register(struct message *msg) {
        std::unique_lock<std::mutex> sem(m2smtx);
        if (!queue_push(m2s_queue, msg)) {
            return -1;
        }
        m2scv.notify_all();
        return 0;
    }

    int master_read_register(struct message *msg) {
        std::unique_lock<std::mutex> sem(s2mmtx);
        while (!queue_pop(s2m_queue, msg)) {
            s2mcv.wait(sem);
        }
        return 0;
    }

    int slave_write_register(struct message *msg) {
        std::unique_lock<std::mutex> sem(s2mmtx);
        if (!queue_push(s2m_queue, msg)) {
            return -1;
        }
        s2mcv.notify_all();
        return 0;
    }

    int slave_read_register(struct message *msg) {
        return !queue_pop(m2s_queue, msg);
    }

  private:
    void thread_cancel(void) {
        reg_start = false;
        m2scv.notify_all();
    }

 #define TE_CHECKPOINT() \
    do { \
        if (false == reg_start) { \
            std::cout << "task exit" << std::endl; \
            return; \
        } \
    } while (0)

    void bus_dispatch_to_slave_entity(void) {
        struct message *msg;
        struct iterator *iter;

        std::cout << "bc task start..." << std::endl;
        while (1) {
            std::unique_lock<std::mutex> sem(m2smtx);
            /* 1. producer & consumer sequence
               2. spurious wakeups */
            while (queue_size(m2s_queue) <= 0) {
                TE_CHECKPOINT();
                m2scv.wait(sem);
            }
            TE_CHECKPOINT();
            iter = bus_message_queue_iterator(m2s_queue);
            while ((msg = (struct message *) iter->next((void *) m2s_queue)) != NULL) {
                device_interrupt_proc(msg->head.meter_id);
            }
        }
        std::cout << "bc task terminated..." << std::endl;
    }

  private:
    bool reg_start = false;
    std::thread task;
    std::mutex s2mmtx;
    std::mutex m2smtx;
    std::condition_variable m2scv;
    std::condition_variable s2mcv;
    struct bus_message_queue *m2s_queue;
    struct bus_message_queue *s2m_queue;
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
