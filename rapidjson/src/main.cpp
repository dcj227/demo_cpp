#include <stdlib.h>
#include <iostream>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>

using namespace std;

void Writer_1() {
    rapidjson::StringBuffer strBuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strBuf);

    writer.StartObject();

    //1. 整数类型
    writer.Key("Int");
    writer.Int(1);

    //2. 浮点类型
    writer.Key("Double");
    writer.Double(12.0000001);

    //3. 字符串类型
    writer.Key("String");
    writer.String("This is a string");

    //4. 结构体类型
    writer.Key("Object");
    writer.StartObject();
    writer.Key("name");
    writer.String("qq849635649");
    writer.Key("age");
    writer.Int(25);
    writer.EndObject();

    //5. 数组类型
    //5.1 整型数组
    writer.Key("IntArray");
    writer.StartArray();
    //顺序写入即可
    writer.Int(10);
    writer.Int(20);
    writer.Int(30);
    writer.EndArray();

    //5.2 浮点型数组
    writer.Key("DoubleArray");
    writer.StartArray();
    for(int i = 1; i < 4; i++)
    {
        writer.Double(i * 1.0);
    }
    writer.EndArray();

    //5.3 字符串数组
    writer.Key("StringArray");
    writer.StartArray();
    writer.String("one");
    writer.String("two");
    writer.String("three");
    writer.EndArray();

    //5.4 混合型数组
    //这说明了，一个json数组内容是不限制类型的
    writer.Key("MixedArray");
    writer.StartArray();
    writer.String("one");
    writer.Int(50);
    writer.Bool(false);
    writer.Double(12.005);
    writer.EndArray();

    //5.5 结构体数组
    writer.Key("People");
    writer.StartArray();
    for(int i = 0; i < 3; i++)
    {
        writer.StartObject();
        writer.Key("name");
        writer.String("qq849635649");
        writer.Key("age");
        writer.Int(i * 10);
        writer.Key("sex");
        writer.Bool((i % 2) == 0);
        writer.EndObject();
    }
    writer.EndArray();

    writer.EndObject();

    std::string data = strBuf.GetString();
    std::cout << data << std::endl;
}

void Writer_2() {
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    //1. 整型类型
    doc.AddMember("Int", 1, allocator);

    //2. 浮点类型
    doc.AddMember("Double", 12.00001, allocator);

    //3. 字符串类型
    //正确方式
    string str= "This is a string";
    rapidjson::Value str_value(rapidjson::kStringType);
    str_value.SetString(str.c_str(), str.size());
    if(!str_value.IsNull())
    {
        doc.AddMember("String", str_value, allocator);
    }
    /**
     *    注：以下方式不正确，可能成功，也可能失败，因为字符串写入json要重新开辟内存，
     * 如果使用该方式的话，当数据是字符串常量的话是没问题的，如果为变量就会显示乱码，所
     * 以为保险起见，我们显式的分配内存(无需释放)
     */
    //doc.AddMember("String", str.data(), allocator);

    //4. 结构体类型
    rapidjson::Value object(rapidjson::kObjectType);
    object.AddMember("name", "qq849635649", allocator); //注：常量是没有问题的
    object.AddMember("age", 25, allocator);
    doc.AddMember("Object", object, allocator);

    //5. 数组类型
    //5.1 整型数组
    rapidjson::Value IntArray(rapidjson::kArrayType);
    IntArray.PushBack(10, allocator);
    IntArray.PushBack(20, allocator);
    IntArray.PushBack(30, allocator);
    doc.AddMember("IntArray", IntArray, allocator);

    //5.2 浮点型数组
    rapidjson::Value DoubleArray(rapidjson::kArrayType);
    DoubleArray.PushBack(1.0, allocator);
    DoubleArray.PushBack(2.0, allocator);
    DoubleArray.PushBack(3.0, allocator);
    doc.AddMember("DoubleArray", DoubleArray, allocator);

    //5.3 字符型数组
    rapidjson::Value StringArray(rapidjson::kArrayType);
    string strValue1 = "one";
    string strValue2 = "two";
    string strValue3 = "three";
    str_value.SetString(strValue1.c_str(), strValue1.size());
    StringArray.PushBack(str_value, allocator);
    str_value.SetString(strValue2.c_str(), strValue2.size());
    StringArray.PushBack(str_value, allocator);
    str_value.SetString(strValue3.c_str(), strValue3.size());
    StringArray.PushBack(str_value, allocator);
    doc.AddMember("StringArray", StringArray, allocator);

    //5.4 结构体数组
    rapidjson::Value ObjectArray(rapidjson::kArrayType);
    for(int i = 1; i < 4; i++)
    {
        rapidjson::Value obj(rapidjson::kObjectType);
        obj.AddMember("name", "qq849635649", allocator);//注：常量是没有问题的
        obj.AddMember("age", i * 10, allocator);
        ObjectArray.PushBack(obj, allocator);
    }
    doc.AddMember("ObjectArray", ObjectArray, allocator);

    {
        rapidjson::StringBuffer strBuf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(strBuf);
        doc.Accept(writer);
        std::string data = strBuf.GetString();
        std::cout << data << std::endl;
    }

    {
        rapidjson::StringBuffer strBuf;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> pretty_writer(strBuf);
        doc.Accept(pretty_writer);
        std::cout << strBuf.GetString() << std::endl;
    }
}

int main() {
    Writer_1();
    Writer_2();
}