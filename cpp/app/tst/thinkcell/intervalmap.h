
#include <stdio.h>
#include <map>
#include <limits>
#include <string>

template<typename K, typename V>
class interval_map {
	std::map<K,V> m_map;

public:
    // constructor associates whole range of K with val by inserting (K_min, val)
    // into the map
    interval_map( V const& val) {
        m_map.insert(m_map.end(),std::make_pair(std::numeric_limits<K>::lowest(),val));
    }

    // Assign value val to interval [keyBegin, keyEnd).
    // Overwrite previous values in this interval.
    // Conforming to the C++ Standard Library conventions, the interval
    // includes keyBegin, but excludes keyEnd.
    // If !( keyBegin < keyEnd ), this designates an empty interval,
    // and assign must do nothing.
    void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
      if (keyEnd<keyBegin)
        return;
      auto bb=m_map.begin();
      auto lb=--m_map.upper_bound(keyBegin);
      if (lb->first==std::numeric_limits<K>::lowest()) {// empty map
        m_map[keyBegin]=val;
        m_map[keyEnd]=V();
      }
      else {
        if (keyBegin<lb->first || lb->second==V())
          m_map.erase(lb);
        m_map[keyBegin]=val;
        lb=m_map.upper_bound(keyBegin);
        std::map<int,int> rmmap;
        while (lb!=m_map.end() && lb->first<keyEnd) {
          rmmap[lb->first]=1;
          lb++;
        }
        auto it=rmmap.begin();
        while (it!=rmmap.end()) {
          m_map.erase(it->first);
          ++it;
        }
        lb=m_map.upper_bound(keyEnd);
        if (lb==m_map.end())
          m_map[keyEnd]=V();
        else
          m_map[keyEnd]=lb->second;

bb=m_map.begin();
while (bb!=m_map.end()) {
  printf("%d [%d,%s]\n",__LINE__,bb->first,bb->second.c_str());
  bb++;
}


        lb=m_map.upper_bound(keyEnd);
        K kk;
        V vv;
        while (lb!=m_map.end()) {
          kk=lb->first;
          vv=lb->second;
          lb++;
          if (lb!=m_map.end())
            m_map[kk]=vv;
          else
            m_map[kk]=V();
        }
      }
      bb=m_map.begin();
      while (bb!=m_map.end()) {
        printf("%d [%d,%s]\n",__LINE__,bb->first,bb->second.c_str());
        bb++;
      }

printf("\n");
    }

    // look-up of the value associated with key
    V const& operator[]( K const& key ) const {
      auto bb=m_map.begin();
      while (bb!=m_map.end()) {
        printf("%d [%d,%s]\n",__LINE__,bb->first,bb->second.c_str());
        bb++;
      }
      return ( --m_map.upper_bound(key) )->second;
    }
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of unsigned int intervals to char.