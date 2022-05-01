#ifndef ENUM_H
#define ENUM_H
#include <string>
enum func_name
{
    AddStudent = 1,
    FlushTable,
    AddRepairlog,
    DelStudent,
    DelRepairlog,
    StudentIn,
    StudentOut,
    ChangePerson,
    Login,
    Register,
    SetUprice,
};

enum table_name
{
    PersonInfo = 1,
    UseInfo,
    RepairLog,
    GymInfo,
};

extern std::string __gymid;
extern int __uprice;
#endif // ENUM_H
