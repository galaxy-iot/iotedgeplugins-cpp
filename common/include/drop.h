#ifndef DROP_H
#define DROP_H

#include "functions.grpc.pb.h"
#include "functions.pb.h"
#include <string>
#include <unordered_map>

class Properties {
public:
  functions::DataType dataType;
  std::vector<std::string> properties;
};

class DeviceDropItem {
public:
  std::string deviceId;
  std::unordered_map<std::string, Properties> properties;
};

class DeviceDrop {
public:
  std::vector<DeviceDropItem> items;

  functions::DeviceDrop *ToFunctionDeviceDrop() {
    functions::DeviceDrop *drop = new functions::DeviceDrop;
    auto mutable_items = drop->mutable_items();

    for (auto it = items.begin(); it != items.end(); it++) {
      std::string deviceId = it->deviceId;
      std::unordered_map<std::string, Properties> propertyMap = it->properties;

      functions::DeviceDropItem *item = new functions::DeviceDropItem;
      auto properties = item->mutable_properties();
      item->set_deviceid(deviceId);

      for (auto pIte = propertyMap.begin(); pIte != propertyMap.end(); pIte++) {
        std::string pName = pIte->first;
        Properties p = pIte->second;

        functions::DataType dataType = p.dataType;
        std::vector<std::string> pValues = p.properties;

        functions::Properties *function_p = new functions::Properties;
        function_p->set_datatype(dataType);
        auto ps = function_p->mutable_properties();

        for (auto pValueIt = pValues.begin(); pValueIt != pValues.end();
             pValueIt++) {
          ps->Add(pValueIt->c_str());
        }

        properties->insert(
            google::protobuf::
                Map<std::basic_string<char>, functions::Properties>::value_type(
                    "p1", *function_p));
      }

      mutable_items->AddAllocated(item);
    }

    return drop;
  }
};

#endif