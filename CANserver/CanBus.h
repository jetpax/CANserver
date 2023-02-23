#ifndef __CANBUS_H__
#define __CANBUS_H__

#include "esp32_can/esp32_can.h"
#include <map>
#include <list>


namespace CANServer 
{
    class CanBus 
    {
    public:

        class AnalysisItem
        {
        public:
            AnalysisItem();
            
            uint32_t frameId;
            uint8_t startBit;
            uint8_t bitLength;
            double factor;
            int signalOffset;
            bool isSigned;
            bool byteOrder;

            float lastValue;
        };

        //This structure allows us to lookup Analysis items quickly by can frame id
        //It just contains pointers to the real stuff.
        typedef std::map<const uint32_t, std::list<AnalysisItem*> > AnalysisItemFrameLookupMap;
        typedef std::pair<const uint32_t, std::list<AnalysisItem*> > AnalysisItemFrameLookupPair;

        //This structure is where we store the actual analysis items so we can look them up by name
        typedef std::map<std::string, AnalysisItem*> AnalysisItemMap;
        typedef std::pair<std::string, AnalysisItem*> AnalysisItemPair;
       
        static CanBus* instance();
        ~CanBus();

        void setup();
        void startup();

        void handle();

        AnalysisItemMap* dynamicAnalysisItems() { return &_analysisItems; };

        void pauseDynamicAnalysis();
        void resumeDynamicAnalysis();

        void saveDynamicAnalysisFile(const char* itemName);
        void deleteDynamicAnalysisFile(const char* itemName);

        void resolveLookups();

    private:
        CanBus();

        static CanBus *_instance;

        CAN_FRAME _workingCANFrame;

        void _processFrame(CAN_FRAME *frame, const uint8_t busId);
        void _processStaticAnalysis(CAN_FRAME *frame, const uint8_t busId);
        void _processDynamicAnalysis(CAN_FRAME *frame, const uint8_t busId);

        void _loadDynamicAnalysisConfiguration();

        bool _dynamicAnalysisPaused;
        AnalysisItemMap _analysisItems;
        AnalysisItemFrameLookupMap _quickFrameIdLookup_analysisItems;
    };
}
#endif