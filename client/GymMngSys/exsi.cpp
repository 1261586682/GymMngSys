#include "exsi.h"

std::vector<int> StoI(const std::string &s)
{
    int len = s.length();
    std::vector<int> ve;
    for (int i = 0; i < len; ++i)
        ve.push_back((int)s[i]);
    return ve;
}

std::string ItoS(const std::vector<int> &ve)
{
    std::string res;
    int len = ve.size();
    for (int i = 0; i < len; ++i)
        res += (char)ve[i];
    return res;
}

std::string AtoS(const google::protobuf::RepeatedField<int32_t>&res)
{
    std::string ans;
    for(int i=0;i<res.size();++i)
        ans+=(char)res[i];
    return ans;
}

int AtoI(char c)
{
    return ((int)c + 256) % 256;
}

bool check_str(QString s)
{
    int len = s.length();
    if(len == 0) return false;
    for(int i=0;i<len;++i)
        if(s[i]==' ')
            return false;
    return true;
}
