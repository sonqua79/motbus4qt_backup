/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://www.modbus4qt.ru
 *
 * $Id: client.h 174 2016-04-27 17:43:13Z l.kolesnik $
 * $URL: https://o.m-i.ru/svn/modbus4qt/branches/1.1/src/client.h $
 *****************************************************************************/

/*****************************************************************************
* The contents of this file are subject to the Mozilla Public License Version 2.0
* (the "License"); you may not use this file except in compliance with the
* License. You may obtain a copy of the License at http://www.mozilla.org/MPL/
*
* Software distributed under the License is distributed on an "AS IS" basis,
* WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
* the specific language governing rights and limitations under the License.
*
* Alternatively, the contents of this file may be used under the terms of the
* GNU General Public License Version 2 or later (the "GPL"), in which case
* the provisions of the GPL are applicable instead of those above. If you wish to
* allow use of your version of this file only under the terms of the GPL and not
* to allow others to use your version of this file under the MPL, indicate your
* decision by deleting the provisions above and replace them with the notice and
* other provisions required by the GPL. If you do not delete the provisions
* above, a recipient may use your version of this file under either the MPL or
* the GPL.
*****************************************************************************/

#ifndef MODBUS4QT_CLIENT_H
#define MODBUS4QT_CLIENT_H

#include <QObject>

#include "global.h"
#include "consts.h"
#include "types.h"

class QIODevice;

namespace modbus4qt
{

/**
* @brief
* @en Abtract modbus client
* @ru Абстрактный клиент протокола modbus
*
* @en
* After call method of data exchange with server forms protocol data unit, and after
* that call method to form application data unit. Formed block will be sent through
* input-output device.
*
* When data sended client waits for a answer and process it after reciveing.
* First process of data recieved will be done in descendant class.
*
* Descendant class must realize abstract methods prepareADU_() and processADU_().
* Also it is important to initialize ioDevice_ field with address of real object resposible
* for data exchange.
*
* @ru
* При вызове метода обмена данными с сервером формирует блок данных протокола
* (Protocol Data Unit) после чего вызывает метод формирования блока данных
* приложения (Apllication Data Unit) класса-потомка. Сформированный блок данных
* передается в устройство ввода-вывода.
*
* После отправки данных серверу ожидает ответа и разбирает полученный ответ.
* Первичная обработка полученного блока данных и выделение полученного блока
* данных протокола происходит в классе-потомке.
*
* Класс-потомок должен релизовать абстрактные методы prepareADU_(),
* processADU_(), а также правильно инициализировать указатель ioDevice_,
* присвоив его значение указателю на экземпляр реального объекта, отвечающего
* за обмен данными.
*
*/
class MODBUS4QT_EXPORT Client : public QObject
{
    Q_OBJECT

    protected:

        /**
         * @brief
         * @en Message about last error occured. Cleared by errorMessage()
         * @ru Текст сообщения о последней возникшей ошибке. Очищается при вызове метода errorMessage()
         *
         * @sa errorMessage();
         */
        QString errorMessage_;

        /**
         * @brief
         * @en IO device for data exchange with server
         * @ru Устройство ввода-вывода, через которое идет обмен данными с сервером
         */
        QIODevice* ioDevice_;

        /**
         * @brief
         * @en Maximum allowed timeout for server responce.
         * @ru Максимальное время ожидания чтения данных от сервера.
         *
         * @en Default value 5000 ms (5 sec).
         * @ru Значение по умолчанию 5000 мс (5 с).
         */
        int readTimeout_;

        /**
         * @brief
         * @en Maximum allowed timeout for sending data to server
         * @ru Максимальное время ожидания запси данных на сервер.
         *
         * @en Default value 5000 ms (5 sec).
         * @ru Значение по умолчанию 5000 мс (5 с).
         */
        int writeTimeout_;

        /**
         * @brief
         * @en Identifier number of server device using for data exchange
         * @ru Идентификатор устройства-сервера, с которым производится обмен данными
         */
        quint8 unitID_;

    protected :

        /**
         * @brief
         * @en Forms protocol data unit
         * @ru Формирует блок данных (Application Data Unit)
         *
         * @en
         * Function should be realized in descendent class since format of application
         * data unit depends from physical commincation line used.
         *
         * @ru
         * Функция должна быть реализована в классе-потомке, так как формат блока
         * данных приложения зависит от используемого канала передачи данных.
         *
         * @param
         * @en pdu - Protocol Data Unit
         * @ru pdu - блок данных протокола (Protocol Data Unit)
         *
         * @param
         * @en pduSize - size of protocol data unit (in bytes)
         * @ru pduSize - размер в байтах блока данных протокола
         *
         * @return
         * @en Application Data Unit formed
         * @ru Сформированный блок данных приложения (Application Data Unit)
         *
         * @sa ProtocolDataUnit, RTUApplicationDataUnit
         *
         */
        virtual QByteArray prepareADU_(const ProtocolDataUnit& pdu, int pduSize) = 0;

        /**
         * @brief
         * @en Process application data unit recieved from server and returns protocol data unit
         * @ru Разбирает полученный от сервера блок данных (Application Data Unit) и возвращает блок данных протокола MODBUS (Protocol Data Unit).
         *
         * @en
         * Function should be realized in descendent class since format of application
         * data unit depends from physical commincation line used.
         *
         * @ru
         * Функция должна быть реализована в классе-потомке, так как формат блока
         * данных приложения зависит от используемого канала передачи данных.
         *
         * @param
         * @en buf - data recieved from server
         * @ru buf - полученный массив данных, полученный от устройства севера
         *
         * @return
         * @en Protocol data unit
         * @ru Блок данных протокола MODBUS
         *
         * @sa ProtocolDataUnit, RTUApplicationDataUnit
         *
         */
        virtual ProtocolDataUnit processADU_(const QByteArray& buf) = 0;

        /**
         * @brief
         * @en Read response from slave device
         * @ru Читает ответ от сервера MODBUS
         *
         * @return
         * @en Readed data array
         * @ru Массив прочитанных данных
         *
         * @en As timeout processing is differ for RTU and TCP we need to implement
         * method in descendance.
         * @ru Так как обработка времени задержки чтения разная для RTU и TCP
         * метод должен быть реализован в классе-потомке.
         */
        virtual QByteArray readResponse_() = 0;

        /**
         * @brief sendRequestToServer_
         * @param requestPDU
         * @param requestPDUSize
         * @param responsePDU
         *
         * @return
         * @en true if request is successfull; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        virtual bool sendRequestToServer_(const ProtocolDataUnit& requestPDU,  int requestPDUSize, ProtocolDataUnit* responsePDU);

        /**
         * @brief sendRequestToServer_
         * @param requestPDU
         * @param requestPDUSize
         *
         * @return
         * @en true if request is successfull; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         * @en It is short version. You can use it if no need to analize reply.
         * @ru Это сокращенная версия. Используется для операций записи, где нет
         * необходимости в анализе ответа от сервера.
         */
        virtual bool sendRequestToServer_(const ProtocolDataUnit& requestPDU,  int requestPDUSize);

    public:
        /**
         * @brief
         * @en Default constructor
         * @ru Конструктор по умолчанию
         *
         * @param
         * @en parent - parent object
         * @ru parent - указатель на объект-родитель.
         */
        explicit Client(QObject *parent = 0);

        /**
         * @brief
         * @en Return last error message and clear it
         * @ru Возвращает значение строки, содержащей сообщение о последней ошибке и очищает его
         *
         * @return
         * @en Last error message
         * @ru Сообщение о последней возникшей ошибке
         *
         */
        QString lastErrorMessage()
        {
            QString result = errorMessage_;
            errorMessage_ = "";

            return result;
        }

        /**
         * @brief
         * @en Read single value from coil
         * @ru Читает значение из регистра дискретного вывода
         *
         * @param
         * @en regNo - coil address
         * @ru regNo - адрес регистра
         *
         * @param
         * @en value - result will be putted here
         * @ru value - переменная для получения результата чтения
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readCoil(quint16 regNo, bool& value);

        /**
         * @brief
         * @en Read array of values from coils
         * @ru Читает массив значений из регистров дискретного вывода
         *
         * @param regNo
         * @en regNo - address of first coil
         * @ru regNo - адрес регистра, с которого необходимо начать чтение
         *
         * @param
         * @en regQty - quantity of coils reading
         * @ru regQty - количество читаемых значений регистров
         *
         * @param
         * @en values - array for values readed
         * @ru values - массив для прочитанных значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readCoils(quint16 regStart, quint16 regQty, QVector<bool>& values);

        /**
         * @brief
         * @en Read array of values from descrete inputs
         * @ru Читает массив значений из регистров дискретного ввода
         *
         * @param
         * @en regStart - address of first register
         * @ru regStart - адрес регистра, с которого необходимо начать чтение
         *
         * @param
         * @en regQty - quantity of inputs reading
         * @ru regQty - количество читаемых значений регистров
         *
         * @param
         * @en values - array for values readed
         * @ru values - массив для прочитанных значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readDescreteInputs(quint16 regStart, quint16 regQty, QVector<bool>& values);

//        /**
//         * @brief readDouble
//         * @param regNo
//         * @param value
//         * @return
//         */
//        bool readDouble(quint16 regNo, double& value);

//        /**
//         * @brief readDWord
//         * @param regNo
//         * @param value
//         * @return
//         */
//        bool readDWord(quint16 regNo, quint32& value);

//        /**
//         * @brief readFloat
//         * @param regNo
//         * @param value
//         * @return
//         *
//         * @sa readSingle()
//         */
//        bool readFloat(quint16 regNo, float& value);

        /**
         * @brief
         * @en Read value from holding register
         * @ru Читает значение из регистра вывода
         *
         * @param regNo
         * @en regNo - address of register
         * @ru regNo - адрес регистра
         *
         * @param
         * @en value - readed value
         * @ru value - прочитанное значение
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readHoldingRegister(quint16 regNo, quint16& value);

        /**
         * @brief
         * @en Read array of values from holding registers
         * @ru Читает массив значений из регистров вывода
         *
         * @param
         * @en regStart - address of first register
         * @ru regStart - адрес регистра, с которого необходимо начать чтение
         *
         * @param
         * @en regQty - quantity of registers reading
         * @ru regQty - количество читаемых значений регистров
         *
         * @param
         * @en values - array for values readed
         * @ru values - массив для прочитанных значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readHoldingRegisters(quint16 regStart, quint16 regQty, QVector<quint16>& values);

        /**
         * @brief
         * @en Read value from input register
         * @ru Читает значение из регистра ввода
         *
         * @param regNo
         * @en regNo - address of register
         * @ru regNo - адрес регистра
         *
         * @param
         * @en value - readed value
         * @ru value - прочитанное значение
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readInputRegister(quint16 regNo, quint16& value);

        /**
         * @brief
         * @en Read array of values from input registers
         * @ru Читает массив значений из регистров ввода
         *
         * @param regNo
         * @en regStart - address of first register
         * @ru regStart - адрес регистра, с которого необходимо начать чтение
         *
         * @param
         * @en regQty - quantity of registers reading
         * @ru regQty - количество читаемых значений регистров
         *
         * @param
         * @en values - array for values readed
         * @ru values - массив для прочитанных значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool readInputRegisters(quint16 regStart, quint16 regQty, QVector<quint16>& values);

//        /**
//         * @brief readSingle
//         * @param regNo
//         * @param value
//         * @return
//         *            @sa readFloat()
//         */
//        bool readSingle(quint16 regNo, float& value /* Single*/);

//        /**
//         * @brief readString
//         * @param regNo
//         * @param length
//         * @return
//         */
//        QString readString(quint16 regNo, quint16 length);

        /**
         * @brief
         * @en Return value for reading timeout
         * @ru Возвращает время ожидания ответа от сервера
         *
         * @return
         * @en Value for reading timeout
         * @ru Время ожидания ответа от сервера
         */
        int readTimeout() const
        {
            return readTimeout_;
        }

        /**
         * @brief
         * @en Set new value for reading timeout
         * @ru Устанавливает время ожидания ответа от сервера
         *
         * @param
         * @en readTimeout - new value for reading timeout, ms
         * @ru readTimeout - максимальное время ожидания ответа от сервера, мс
         */
        void setReadTimeOut(int readTimeout)
        {
            readTimeout_ = readTimeout;
        }

        /**
         * @brief
         * @en Set current value of server unit ID
         * @ru Устанавливает текущее значение адреса устройства сервера, с которым идет обмен данными
         *
         * @param
         * @en unitID - server device unit id
         * @ru unitID - адрес устройства сервера
         */
        void setUnitID(quint8 unitID)
        {
            unitID_ = unitID;
        }

        /**
         * @brief
         * @en Set new value for reading timeout
         * @ru Устанавливает время ожидания ответа от сервера
         *
         * @param
         * @en readTimeout - new value for reading timeout, ms
         * @ru readTimeout - максимальное время ожидания ответа от сервера, мс
         */
        void setWriteTimeOut(int writeTimeout)
        {
            writeTimeout_ = writeTimeout;
        }

        /**
         * @brief
         * @en Write value to coil
         * @ru Записывает значение в дискретный выход
         *
         * @param regNo
         * @en regNo - address of coil
         * @ru regNo - адрес выхода, в который производится запись
         *
         * @param
         * @en value - value to write
         * @ru value - записываемое значение
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         * @en It is alias of writeSingleCoil() method for convinience.
         * @ru Этот метод является псевдонимом метода writeSingleCoil().
         */
        bool writeCoil(quint16 regNo, bool value)
        {
            return writeSingleCoil(regNo, value);
        }

        /**
         * @brief
         * @en Write array of values to coils
         * @ru Записывает массив значений в дискретные выходы
         *
         * @param
         * @en regStart - address of first coil
         * @ru regStart - адрес выхода, с которого необходимо начать запись
         *
         * @param
         * @en values - array for values to write
         * @ru values - массив записываемых значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         * @en It is alias of writeMultipleCoils() method for convinience.
         * @ru Этот метод является псевдонимом метода writeMultipleCoils().
         */
        bool writeCoils(quint16 regStart, const QVector<bool>& values)
        {
            return writeMultipleCoils(regStart, values);
        }

        /**
         * @brief
         * @en Write value to holding register
         * @ru Записывает значение в регистр вывода
         *
         * @param
         * @en regAddress - address of register
         * @ru regAddress - адрес регистра
         *
         * @param
         * @en value - value to write
         * @ru value - записываемое значение
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         * @en It is alias of writeSingleRegister() method for convinience.
         * @ru Этот метод является псевдонимом метода writeSingleRegister().
         *
         */
        bool writeHoldingRegister(quint16 regAddress, quint16 value)
        {
            return writeSingleRegister(regAddress, value);
        }

        /**
         * @brief
         * @en Write array of values to holding registers
         * @ru Записывает массив значений в регистры вывода
         *
         * @param
         * @en regStart - address of first register
         * @ru regStart - адрес регистра, с которого необходимо начать запись
         *
         * @param
         * @en values - array for values to write
         * @ru values - массив записываемых значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         * @en It is alias of writeMultipleRegisters() method for convinience.
         * @ru Этот метод является псевдонимом метода writeMultipleRegisters().
         *
         */
        bool writeHoldingRegisters(quint16 regStart, const QVector<quint16>& values)
        {
            return writeMultipleRegisters(regStart, values);
        }

        /**
         * @brief
         * @en Write array of values to coils
         * @ru Записывает массив значений в дискретные выходы
         *
         * @param
         * @en regStart - address of first coil
         * @ru regStart - адрес выхода, с которого необходимо начать запись
         *
         * @param
         * @en values - array for values to write
         * @ru values - массив записываемых значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool writeMultipleCoils(quint16 regStart, const QVector<bool>& values);

        /**
         * @brief
         * @en Write array of values to holding registers
         * @ru Записывает массив значений в регистры вывода
         *
         * @param
         * @en regStart - address of first register
         * @ru regStart - адрес регистра, с которого необходимо начать запись
         *
         * @param
         * @en values - array for values to write
         * @ru values - массив записываемых значений
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool writeMultipleRegisters(quint16 regStart, const QVector<quint16>& values);

        //        /**
//         * @brief writeDouble
//         * @param regNo
//         * @param value
//         * @return
//         */
//        bool writeDouble(quint16 regNo, double value);

//        /**
//         * @brief writeDWord
//         * @param regNo
//         * @param value
//         * @return
//         */
//        bool writeDWord(quint16 regNo, quint32 value);

//        /**
//         * @brief writeFloat
//         * @param regNo
//         * @param value
//         * @return
//         */
//        bool writeFloat(quint16 regNo, float value)
//        {
//            return writeSingle(regNo, value);
//        }

        /**
         * @brief
         * @en Write value to coil
         * @ru Записывает значение в дискретный выход
         *
         * @param
         * @en regAddress - address of coil
         * @ru regAddress - адрес выхода, в который производится запись
         *
         * @param
         * @en value - value to write
         * @ru value - записываемое значение
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool writeSingleCoil(quint16 regAddress, bool value);

        /**
         * @brief
         * @en Write value to holding register
         * @ru Записывает значение в регистр вывода
         *
         * @param
         * @en regAddress - address of register
         * @ru regAddress - адрес регистра
         *
         * @param
         * @en value - value to write
         * @ru value - записываемое значение
         *
         * @return
         * @en true in success; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         *
         */
        bool writeSingleRegister(quint16 regAddress, quint16 value);

//        /**
//         * @brief writeSingle
//         * @param regNo
//         * @param value
//         * @return
//         */
//        bool writeSingle(quint16 regNo, float value /* Single*/);

//        /**
//         * @brief writeString
//         * @param regNo
//         * @param text
//         * @return
//         */
//        bool writeString(quint16 regNo, const QString& text);

        /**
         * @brief
         * @en Return value for writing timeout
         * @ru Возвращает время ожидания передачи данных серверу
         *
         * @return
         * @en Value for reading timeout
         * @ru Время ожидания передачи данных серверу
         */
        int writeTimeout() const
        {
            return writeTimeout_;
        }

        /**
         * @brief
         * @en Return current value of server unit ID
         * @ru Возвращает текущее значение адреса устройства сервера, с которым идет обмен данными
         *
         * @return
         * @en Server unit id
         * @ru Адрес устройства сервера
         */
        quint8 unitID() const
        {
            return unitID_;
        }

        /**
         * @brief
         * @en Send user defined function command to server
         * @ru Отправляет на сервер команду функции, определяемой пользователем (производителем)
         *
         * @param
         * @en function - Function code
         * @ru function - Код функции
         *
         * @param
         * @en data - Extra data to be sended to server
         * @ru data - Дополнительные данные, передаваемые серверу
         *
         * @param
         * @en retData - Vector to plase data returned from server
         * @ru retData - Массив, в который будет помещены данные, полученные от сервера
         *
         * @return
         * @en true if function successfull; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         */
        bool userDefinedFunction(quint8 function, const QVector<quint8>& data, QVector<quint8>& retData);

        /**
         * @brief
         * @en Send user defined function command to server
         * @ru Отправляет на сервер команду функции, определяемой пользователем (производителем)
         *
         * @param
         * @en function - Function code
         * @ru function - Код функции
         *
         * @param
         * @en subFunction - Sub function code
         * @ru subFunction - Код подфункции
         *
         * @param
         * @en data - Extra data to be sended to server
         * @ru data - Дополнительные данные, передаваемые серверу
         *
         * @param
         * @en retData - Vector to plase data returned from server
         * @ru retData - Массив, в который будет помещены данные, полученные от сервера
         *
         * @return
         * @en true if function successfull; false otherwise
         * @ru true если запрос прошел успешно; false в случае возникновения ошибки
         */
        bool userDefinedFunction(quint8 function, quint8 subFunction, const QVector<quint8>& data, QVector<quint8>& retData);

    signals:

        /**
         * @brief
         * @en Signal for debuggin info
         * @ru Сигнал для вывода отладочного сообщения
         *
         * @param
         * @en msg - Debug message
         * @ru msg - Строка с отладочным сообщение
         */
        void debugMessage(const QString& msg);

        /**
         * @brief
         * @en Signal for informing about error occured
         * @ru Сигнал для сообщения о возникновении ошибки
         *
         * @param
         * @en msg - Message with error description
         * @ru msg - Строка с описанием ошибки
         */
        void errorMessage(const QString& msg);

        /**
         * @brief
         * @en Signal for informing about error occured
         * @ru Сигнал для сообщения о возникновении ошибки
         *
         * @param
         * @en unitID - unit ID of device
         * @en unitID - адрес устройства, при обмене данными с которым произошла ошибка
         *
         * @en msg - Message with error description
         * @ru msg - Строка с описанием ошибки
         */
        void errorMessage(quint8 unitID, const QString& msg);

        /**
         * @brief
         * @en Signal for informing about error occured
         * @ru Сигнал для сообщения о возникновении ошибки
         *
         * @param
         * @en msg - Message with error description
         * @ru msg - Строка с описанием ошибки
         */
        void infoMessage(const QString& msg);

        /**
         * @brief
         * @en Signal for informing about error occured
         * @ru Сигнал для сообщения о возникновении ошибки
         *
         * @param
         * @en unitID - unit ID of device
         * @en unitID - адрес устройства, при обмене данными с которым произошла ошибка
         *
         * @en msg - Message with error description
         * @ru msg - Строка с описанием ошибки
         */
        void infoMessage(quint8 unitID, const QString& msg);
};

} // namespace modbus4qt

#endif // MODBUS4QT_CLIENT_H
