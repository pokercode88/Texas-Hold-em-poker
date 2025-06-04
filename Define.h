// Serialize example
// This example shows writing JSON string with writer directly.
#ifndef __DEFINE_H__
#define __DEFINE_H__


#include "rapidjson/writer.h" // for stringify JSON
#include "rapidjson/document.h"

#include <cstdio>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include "globe.h"


using namespace rapidjson;
using namespace std;

class CString;
class CInteger;
class CString
{
public:
    CString()
    {
        this->_isNull = true;
    }

    CString(const std::string &str)
    {
        this->_data = str;
        this->_isNull = false;
    }
    std::string toString()
    {
        if (isNull())
        {
            throw logic_error("CString is NULL");
        }
        return this->_data;
    }
    void assign(const std::string &str)
    {
        this->_data = str;
        this->_isNull = false;
    }

    void assign(const CString &rhs)
    {
        this->_data = rhs._data;
        this->_isNull = rhs._isNull;
    }

    operator std::string()const
    {
        if (isNull())
        {
            throw logic_error("CString is NULL");
        }
        return this->_data;
    }

    bool isNull()const
    {
        return this->_isNull;
    }
private:
    std::string _data;
    bool _isNull;
};

class CInteger
{
public:
    CInteger()
    {
        this->_isNull = true;
    }

    CInteger(int64_t value)
    {
        this->_data = value;
        this->_isNull = false;
    }
    std::string toString()
    {
        if (isNull())
        {
            throw logic_error("CInteger is NULL");
        }
        return TC_Common::tostr<int64_t>(this->_data);
    }
    void assign(int64_t newValue)
    {
        this->_data = newValue;
        this->_isNull = false;
    }

    void assign(const  CInteger &newValue)
    {
        this->_data = newValue._data;
        this->_isNull = newValue._isNull;
    }

    void assign(const string &newValue)
    {
        int64_t intValue = S2L(newValue);
        this->_data = intValue;
        this->_isNull = false;
    }

    operator int64_t() const
    {
        if (isNull())
        {
            throw logic_error("CInteger is NULL");
        }
        return this->_data;
    }

    bool isNull()const
    {
        return this->_isNull;
    }
private:
    int64_t _data;
    bool _isNull;
};


class CDouble
{
public:
    CDouble()
    {
        this->_isNull = true;
    }

    CDouble(double value)
    {
        this->_data = value;
        this->_isNull = false;
    }
    std::string toString()
    {
        if (isNull())
        {
            throw logic_error("CDouble is NULL");
        }
        return TC_Common::tostr<double>(this->_data);
    }
    void assign(double newValue)
    {
        this->_data = newValue;
        this->_isNull = false;
    }

    void assign(const  CDouble &newValue)
    {
        this->_data = newValue._data;
        this->_isNull = newValue._isNull;
    }

    void assign(const string &newValue)
    {
        double intValue = TC_Common::strto<double>(newValue);
        this->_data = intValue;
        this->_isNull = false;
    }

    operator double() const
    {
        if (isNull())
        {
            throw logic_error("CInteger is NULL");
        }
        return this->_data;
    }

    bool isNull()const
    {
        return this->_isNull;
    }
private:
    double _data;
    bool _isNull;
};

// 管理后台请求状态码
const static int64_t RESULT_CODE_SUCCESS	= 1;
const static int64_t RESULT_CODE_FAIL		= 0;
#define INSERT_ASSIGN_STATEMENT(member) req.data[#member] = request._##member.toString()
#define UPDATE_ASSIGN_STATEMENT(member) 			if (request._##member.isNull() == false) \
													{ \
														req.data[#member] = request._##member.toString(); \
													}
#define SERIALIZE_MEMBER(writer, member) serializeMember(writer,#member,_##member)
#define SET_DOC_MEMBER(document, member) setDocMember(document, #member, _##member);
#define SET_RESPONSE_MEMBER(member) 				if (key == #member) { \
													response._##member.assign(value); \
													}
template <typename Writer>
void serializeMember(Writer &writer, const CString &content)
{
    if (content.isNull())
    {
        writer.Null();
    }
    else
    {
        std::string str = content;
        writer.String(str.c_str(), (SizeType)str.length());
    }
}

template <typename Writer>
void serializeMember(Writer &writer, const CInteger &content)
{
    if (content.isNull())
    {
        writer.Null();
    }
    else
    {
        writer.Int64(content);
    }
}

template <typename Writer>
void serializeMember(Writer &writer, const CDouble &content)
{
    if (content.isNull())
    {
        writer.Null();
    }
    else
    {
        writer.Double(content);
    }
}

template <typename Writer, typename MemberType>
void serializeMember(Writer &writer, const string &keyName, const MemberType &member)
{
    writer.String(keyName.c_str());
    serializeMember(writer, member);
}

static void setDocMember(const Document &d, const std::string &keyName, CString &member)
{
    if (
        d.HasMember(keyName.c_str()) == false ||
        (d[keyName.c_str()].IsString() == false && d[keyName.c_str()].IsNull() == false)
    )
    {
        throw logic_error("json Type error!");
    }
    if (d[keyName.c_str()].IsString())
    {
        member.assign(d[keyName.c_str()].GetString());
    }
}

static void setDocMember(const Document &d, const std::string &keyName, CInteger &member)
{
    if (
        d.HasMember(keyName.c_str()) == false ||
        (d[keyName.c_str()].IsInt64() == false && d[keyName.c_str()].IsNull() == false)
    )
    {
        throw logic_error("json Type error!");
    }
    if (d[keyName.c_str()].IsInt64())
    {
        member.assign(d[keyName.c_str()].GetInt64());
    }
}

static void setDocMember(const Document &d, const std::string &keyName, CDouble &member)
{
    if (
        d.HasMember(keyName.c_str()) == false ||
        (d[keyName.c_str()].IsDouble() == false && d[keyName.c_str()].IsNull() == false)
    )
    {
        throw logic_error("json Type error!");
    }
    if (d[keyName.c_str()].IsDouble())
    {
        member.assign(d[keyName.c_str()].GetDouble());
    }
}

class GMRequest
{
public:
    GMRequest() {}
    GMRequest(const std::string &code, const std::string &params, int64_t time, const std::string &sign) :
        _code(code),
        _params(params),
        _time(time),
        _sign(sign) {}

    template <typename Writer>
    void Serialize(Writer &writer) const
    {
        writer.StartObject();
        SERIALIZE_MEMBER(writer, code);
        SERIALIZE_MEMBER(writer, params);
        SERIALIZE_MEMBER(writer, time);
        SERIALIZE_MEMBER(writer, sign);
        writer.EndObject();
    }

    void toString(std::string &json)
    {
        StringBuffer sb;
        Writer<StringBuffer> writer(sb);
        Serialize(writer);
        json = sb.GetString();
    }

    void Deserialize(const string &json)
    {
        try
        {
            Document d;
            if (d.Parse(json.c_str()).HasParseError())
            {
                throw logic_error("parse json error. raw data : " + json);
            }
            //赋值
            SET_DOC_MEMBER(d, code);
            SET_DOC_MEMBER(d, params);
            SET_DOC_MEMBER(d, time);
            SET_DOC_MEMBER(d, sign);
        }
        catch (const std::exception &e)
        {
            std::string errInfo = std::string(__FILE__) + ":" + std::to_string(__LINE__) + ":GMRequest decode error!";
            throw logic_error(errInfo);
        }
    }

    void getParams(vector<tars::Char> &params)
    {
        std::string str;
        if(this->_params.isNull())
        {
            str = "";
        }
        else
        {
            str = this->_params;
        }
        params.assign(str.begin(), str.end());
    }

    std::string getParams()
    {
        std::string str = this->_params;
        return str;
    }

    int getCode() const
    {
        if (this->_code.isNull())
        {
            throw logic_error("code Null!");
        }

        std::string code = this->_code;
        return S2I(code);
    }
private:
    CString		_code;
    CString		_params;
    CInteger	_time;
    CString		_sign;
};


class GMResponse
{
public:
    GMResponse() {}
    GMResponse(int64_t result, const std::string &msg, const std::string &resultData, int64_t totalItems, int64_t totalPages) :
        _result(result),
        _msg(msg),
        _resultData(resultData),
        _totalItems(totalItems),
        _totalPages(totalPages) {}

    // 删除响应使用的构造函数
    GMResponse(int64_t result, const std::string &msg) :
        _result(result),
        _msg(msg) {}

    // 插入和更新响应使用的构造函数
    GMResponse(int64_t result, const std::string &msg, const std::string &resultData) :
        _result(result),
        _msg(msg),
        _resultData(resultData) {}
    template <typename Writer>
    void Serialize(Writer &writer) const
    {
        writer.StartObject();
        SERIALIZE_MEMBER(writer, result);
        SERIALIZE_MEMBER(writer, msg);
        SERIALIZE_MEMBER(writer, resultData);
        SERIALIZE_MEMBER(writer, totalItems);
        SERIALIZE_MEMBER(writer, totalPages);
        writer.EndObject();
    }

    void toString(std::string &json)
    {
        StringBuffer sb;
        Writer<StringBuffer> writer(sb);
        Serialize(writer);
        json = sb.GetString();
    }

    void Deserialize(const string &json)
    {
        Document d;
        if (d.Parse(json.c_str()).HasParseError())
        {
            throw logic_error("parse json error. raw data : " + json);
        }
        SET_DOC_MEMBER(d, result);
        SET_DOC_MEMBER(d, msg);
        SET_DOC_MEMBER(d, resultData);
        SET_DOC_MEMBER(d, totalItems);
        SET_DOC_MEMBER(d, totalPages);
    }
private:
    CInteger		_result;
    CString			_msg;
    CString			_resultData;
    CInteger		_totalItems;
    CInteger		_totalPages;
};

#endif // !__DEFINE_H