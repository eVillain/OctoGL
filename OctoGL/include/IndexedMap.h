#ifndef INDEXED_MAP_H
#define INDEXED_MAP_H

#include <map>
#include <vector>

/// Key-value container which recycles the keys when removing entries
/// Do try not to add too many entries or else :)
template <typename Key, typename Value>
class IndexedMap
{
public:
    ~IndexedMap()
    {
        _recycledIDs.clear();
        _map.clear();
    }

    Key add(Value value)
    {
        Key newID = GetNextID();
        _map[newID] = value;
        return newID;
    }
    
    void remove(Key key)
    {
        auto it = _map.find (key);
        if (it != _map.end()) {
            _map.erase(it);
            _recycledIDs.push_back(key);
        }
    }
    
    const std::map<Key, Value> getMap()
    {
        return _map;
    }
    
private:
    std::map<Key, Value> _map;

    Key _nextID;
    std::vector<Key> _recycledIDs;
    
    Key GetNextID()
    {
        if (!_recycledIDs.empty()) {
            Key recycledID = _recycledIDs.back();
            _recycledIDs.pop_back();
            return recycledID;
        }
        if ((_nextID + 1) < _nextID) // Value wrap-around sanity check
        {
            // TODO: Throw a proper exception here, or maybe use an assert above
            printf("[IndexedMap] Ran out of IDs!");
            // Bad things follow this point, for now ^_°
        }
        return _nextID++;
    }
};
#endif /* INDEXED_MAP_H */
