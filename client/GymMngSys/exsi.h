#ifndef EXSI_H
#define EXSI_H

#include <vector>
#include <string>
#include <QString>
#include <proto.pb.h>

std::vector<int> StoI(const std::string &s);

std::string ItoS(const std::vector<int> &ve);

std::string AtoS(const google::protobuf::RepeatedField<int32_t>&);

bool check_str(QString);

int AtoI(char c);
#endif // EXSI_H
