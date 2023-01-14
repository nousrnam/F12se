#ifndef CITYTXT_H
#define CITYTXT_H

#include <vector>
#include <string>

class IniFile;
class MemBuffer;

class CityTxt
   {
public:
   size_t Count() const {return records.size();}
   void Load(IniFile &iniFile);
   void Load(const MemBuffer &buf);
   void Clear(){records.clear();};
   bool empty() const {return records.empty();}
   std::string Name(size_t cityNum) const {return records[cityNum].name;};
   size_t EntrNum(size_t cityNum) const {return records[cityNum].entrances.size();};
   bool EntrStartOn(size_t cityNum, size_t entrNum) const {return records[cityNum].entrances[entrNum].startStateOn;}
   bool CityStartOn(size_t cityNum) const {return records[cityNum].startStateOn;}
private:
    struct Entrance
        {
        Entrance(bool startOn) : startStateOn(startOn) {}
        bool startStateOn;
        };
    struct CityRec
        {
        std::string name;
        std::vector<Entrance> entrances;
        bool startStateOn;
        };
   std::vector<CityRec> records;
   };

#endif // CITYTXT_H
