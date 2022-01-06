# Welcome to PowerDale

PowerDale is a small town with around 100 residents. Most houses have a smart meter installed that can save and send
information about how much power a house is drawing/using.

There are three major providers of energy in town that charge different amounts for the power they supply.

- _Dr Evil's Dark Energy_
- _The Green Eco_
- _Power for Everyone_

# Introducing JOI Energy

JOI Energy is a new start-up in the energy industry. Rather than selling energy they want to differentiate themselves
from the market by recording their customers' energy usage from their smart meters and recommending the best supplier to
meet their needs.

You have been placed into their development team, whose current goal is to produce an API which their customers and
smart meters will interact with.

Unfortunately, two members of the team are on annual leave, and another one has called in sick! You are left with
another ThoughtWorker to progress with the current user stories on the story wall. This is your chance to make an impact
on the business, improve the code base and deliver value.

## Story Wall

At JOI energy the development team use a story wall or Kanban board to keep track of features or "stories" as they are
worked on.

The wall you will be working from today has 7 columns:

- Backlog
- Ready for Dev
- In Dev
- Ready for Testing
- In Testing
- Ready for sign off
- Done

Examples can be found
here [https://leankit.com/learn/kanban/kanban-board/](https://leankit.com/learn/kanban/kanban-board/)

## Users

To trial the new JOI software 5 people from the JOI accounts team have agreed to test the service and share their energy
data.

| User    | Smart Meter ID  | Power Supplier        |
| ------- | --------------- | --------------------- |
| Sarah   | `smart-meter-0` | Dr Evil's Dark Energy |
| Peter   | `smart-meter-1` | The Green Eco         |
| Charlie | `smart-meter-2` | Dr Evil's Dark Energy |
| Andrea  | `smart-meter-3` | Power for Everyone    |
| Alex    | `smart-meter-4` | The Green Eco         |

These values are used in the code and in the following examples too.

## Requirements

The project requires Cmake 3.21 or
higher.

The project makes use of Conan.

## Useful Conan commands

The project makes use of Conan to help you out carrying some common tasks such as building
the project or running it.

### Build the project

Compiles the project, runs the test and then creates an executable file.

```shell
$ mkdir build && cd build
$ conan install .. --build missing
$ cmake ..
$ cmake --build . -j
```

### Run the tests

Technically, this app is a firmware, which is unable to be run on PC, but you can run test.

There are two types of tests, the unit tests and the functional tests. These can be executed as follows.

- Run functional tests only

  ```console
  $ ctest .
  ```

## API

Smart meter accepts API calls from kind of endpoint (could be UART, IIC, SPI etc...), request and respond datagram
follows format:

```
| byte 1 | byte 2 | byte 3 | byte 4 | byte 5 | byte 6 | byte 7 | byte 8 |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
|                                meter                                  |
|                                 id                                    |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
|               length              |              command              |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
|                                payload...                             |
```

### Store Readings

There no API to trigger a reading storing, smart meter will store a new reading record every 15 minutes by default.

### Get Stored Readings

Reading command

```
| byte 1 | byte 2 | byte 3 | byte 4 | byte 5 | byte 6 | byte 7 | byte 8 |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
|                                meter                                  |
|                                 id                                    |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
|               length              |              command              |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
```

Command is: MESSAGE_READINGS_READ

Response comes with the same message head, but with payload which is defined by `struct reading_message_response`
in `meter/adapter/protocol/reading.h`.

### View Current Price Plan and Compare Usage Cost Against all Price Plans

Price plan compare command:

```
| byte 1 | byte 2 | byte 3 | byte 4 | byte 5 | byte 6 | byte 7 | byte 8 |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
|                                meter                                  |
|                                 id                                    |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
|               length              |              command              |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
```

Command is: MESSAGE_PRICE_PLAN_COMPARE_ALL

Response comes with the same message head, but with payload which is defined by `struct price_plan_compare_all_response`
in `meter/adapter/protocol/price_plan.h`.

### View Recommended Price Plans for Usage

Price plan recommend command:

```
| byte 1 | byte 2 | byte 3 | byte 4 | byte 5 | byte 6 | byte 7 | byte 8 |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
|                                meter                                  |
|                                 id                                    |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
|               length              |              command              |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ |
|               limit               |
| ------ | ------ | ------ | ------ |
```

Command is: MESSAGE_PRICE_PLAN_RECOMMEND

Response comes with the same message head, but with payload which is defined by `struct price_plan_recommend_response`
in `meter/adapter/protocol/price_plan.h`.
