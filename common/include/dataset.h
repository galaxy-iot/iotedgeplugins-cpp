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

class DeviceDataSetItem {
public:
  std::string deviceId;
  std::unordered_map<std::string, Properties> properties;
};

class DeviceDataSet {
public:
  std::vector<DeviceDataSetItem> items;

  functions::DeviceDataSet *ToFunctionDeviceDrop() {
    functions::DeviceDataSet *ds = new functions::DeviceDataSet;
    auto mutable_items = ds->mutable_items();

    for (auto it = items.begin(); it != items.end(); it++) {
      std::string deviceId = it->deviceId;
      std::unordered_map<std::string, Properties> propertyMap = it->properties;

      functions::DeviceDataSetItem *item = new functions::DeviceDataSetItem;
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

    return ds;
  }
};

#endif