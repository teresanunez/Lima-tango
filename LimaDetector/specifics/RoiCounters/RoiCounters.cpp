static const char *RcsId = "$Id:  $";
//+=============================================================================
//
// file :         RoiCounters.cpp
//
// description :  C++ source for the RoiCounters and its commands. 
//                The class is derived from Device. It represents the
//                CORBA servant object which will be accessed from the
//                network. All commands which can be executed on the
//                RoiCounters are implemented in this file.
//
// project :      TANGO Device Server
//
// $Author:  $
//
// $Revision:  $
//
// $Revision:  $
// $Date:  $
//
// SVN only:
// $HeadURL: $
//
// CVS only:
// $Source:  $
// $Log:  $
//
// copyleft :    Synchrotron SOLEIL 
//               L'Orme des merisiers - Saint Aubin
//               BP48 - 91192 Gif sur Yvette
//               FRANCE
//
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================



//===================================================================
//
//	The following table gives the correspondence
//	between commands and method name.
//
//  Command name|  Method name
//	----------------------------------------
//  State   |  dev_state()
//  Status  |  dev_status()
//
//===================================================================


#ifdef WIN32
#include <tango.h>
#include <PogoHelper.h>
#endif

#include <RoiCounters.h>
#include <RoiCountersClass.h>

#ifndef WIN32
#include <tango.h>
#include <PogoHelper.h>
#endif



namespace RoiCounters_ns
{

//+----------------------------------------------------------------------------
//
// method : 		RoiCounters::RoiCounters(string &s)
// 
// description : 	constructor for simulated RoiCounters
//
// in : - cl : Pointer to the DeviceClass object
//      - s : Device name 
//
//-----------------------------------------------------------------------------

RoiCounters::RoiCounters(Tango::DeviceClass *cl, string &s)
:Tango::Device_4Impl(cl, s.c_str()), m_dim(this)
{
    init_device();
}

RoiCounters::RoiCounters(Tango::DeviceClass *cl, const char *s)
:Tango::Device_4Impl(cl, s), m_dim(this)
{
    init_device();
}

RoiCounters::RoiCounters(Tango::DeviceClass *cl, const char *s, const char *d)
:Tango::Device_4Impl(cl, s, d), m_dim(this)
{
    init_device();
}

//+----------------------------------------------------------------------------
//
// method : 		RoiCounters::delete_device()
// 
// description : 	will be called at device destruction or at init command.
//
//-----------------------------------------------------------------------------

void RoiCounters::delete_device()
{
    INFO_STREAM << "RoiCounters::delete_device() delete device " << device_name << endl;
    yat::AutoMutex<> _lock(ControlFactory::instance().get_global_mutex());
    //	Delete device allocated objects	
    DELETE_DEVSTRING_ATTRIBUTE(attr_version_read);

    //- remove any dynamic attr or command
    INFO_STREAM << "remove any dynamic attributes or commands" << endl;
    m_dim.remove();
}

//+----------------------------------------------------------------------------
//
// method : 		RoiCounters::init_device()
// 
// description : 	will be called at device initialization.
//
//-----------------------------------------------------------------------------

void RoiCounters::init_device()
{
    INFO_STREAM << "RoiCounters::init_device() create device " << device_name << endl;
    yat::AutoMutex<> _lock(ControlFactory::instance().get_global_mutex());
    // Initialise variables to default values
    //--------------------------------------------	
    get_device_property();

    CREATE_DEVSTRING_ATTRIBUTE(attr_version_read, 256);
    //By default INIT, need to ensure that all objets are OK before set the device to STANDBY
    set_state(Tango::INIT);
    m_is_device_initialized = false;
    m_status_message.str("");

    try
    {
        yat::AutoMutex<> _lock(ControlFactory::instance().get_global_mutex());
        //- get the main object used to pilot the lima framework
        //in fact LimaDetector is create the singleton control objet
        m_ct = ControlFactory::instance().get_control("RoiCounters");
    }
    catch(Exception& e)
    {
        INFO_STREAM << "Initialization Failed : " << e.getErrMsg() << endl;
        m_status_message << "Initialization Failed : " << e.getErrMsg() << endl;
        m_is_device_initialized = false;
        set_state(Tango::FAULT);
        return;
    }
    catch(...)
    {
        INFO_STREAM << "Initialization Failed : UNKNOWN" << endl;
        m_status_message << "Initialization Failed : UNKNOWN" << endl;
        set_state(Tango::FAULT);
        m_is_device_initialized = false;
        return;
    }

    //---------------------------------------------------------
    //Configure/create all dynamic attributes
    //---------------------------------------------------------
    if(!create_all_dynamic_attributes())
    {
        m_is_device_initialized = false;
        return;
    }

    try
    {
        //create new operation			
        std::stringstream opId("MyRoiCounters");
        INFO_STREAM << "addOp(" << opId.str() << ")" << endl;
        m_ct->externalOperation()->addOp(ROICOUNTERS, opId.str(), 0/*level*/, m_soft_operation);

        //Write tango hardware at Init
        std::stringstream ssName;
        std::string strName;
        for(size_t i = 0; i < nbRoiCounters; i++)
        {
            //get memorized from properties
            attr_x_array[i] = (Tango::DevULong)(__x.at(i));
            attr_y_array[i] = (Tango::DevULong)(__y.at(i));
            attr_width_array[i] = (Tango::DevULong)(__width.at(i));
            attr_height_array[i] = (Tango::DevULong)(__height.at(i));
        
            ssName.str("");
            ssName << "x" << i;
            strName = ssName.str();
            INFO_STREAM << "Write tango hardware at Init - " << strName << endl;
            Tango::WAttribute &x = dev_attr->get_w_attr_by_name(strName.c_str());
            x.set_write_value(attr_x_array[i]);
            yat4tango::DynamicAttributeWriteCallbackData cbd_x;
            cbd_x.tga = &x;
            write_rois_callback(cbd_x);
        
            ssName.str("");
            ssName << "y" << i;
            strName = ssName.str();
            INFO_STREAM << "Write tango hardware at Init - " << strName << endl;
            Tango::WAttribute &y = dev_attr->get_w_attr_by_name(strName.c_str());
            y.set_write_value(attr_y_array[i]);
            yat4tango::DynamicAttributeWriteCallbackData cbd_y;
            cbd_y.tga = &y;
            write_rois_callback(cbd_y);
        
            ssName.str("");
            ssName << "width" << i;
            strName = ssName.str();
            INFO_STREAM << "Write tango hardware at Init - " << strName << endl;
            Tango::WAttribute &width = dev_attr->get_w_attr_by_name(strName.c_str());
            width.set_write_value(attr_width_array[i]);
            yat4tango::DynamicAttributeWriteCallbackData cbd_width;
            cbd_width.tga = &width;
            write_rois_callback(cbd_width);
                   
            ssName.str("");
            ssName << "height" << i;
            strName = ssName.str();
            INFO_STREAM << "Write tango hardware at Init - " << strName << endl;
            Tango::WAttribute &height = dev_attr->get_w_attr_by_name(strName.c_str());
            height.set_write_value(attr_height_array[i]);
            yat4tango::DynamicAttributeWriteCallbackData cbd_height;
            cbd_height.tga = &height;
            write_rois_callback(cbd_height);
        }
    }
    catch(Exception& e)
    {
        ERROR_STREAM << "Initialization Failed : " << e.getErrMsg() << endl;
        m_status_message << "Initialization Failed : " << e.getErrMsg() << endl;
        m_is_device_initialized = false;
        set_state(Tango::FAULT);
        return;
    }
    catch(Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        m_status_message << "Initialization Failed : ";
        for(unsigned i = 0; i < df.errors.length(); i++)
        {
            m_status_message << df.errors[i].desc << endl;
        }
        m_is_device_initialized = false;
        set_state(Tango::FAULT);
        return;
    }
    catch(...)
    {
        ERROR_STREAM << "Initialization Failed : UNKNOWN" << endl;
        m_status_message << "Initialization Failed : UNKNOWN" << endl;
        set_state(Tango::FAULT);
        m_is_device_initialized = false;
        return;
    }

    //everything is Ok
    m_is_device_initialized = true;

    set_state(Tango::STANDBY);
    dev_state();
}

//+----------------------------------------------------------------------------
//
// method : 		RoiCounters::get_device_property()
// 
// description : 	Read the device properties from database.
//
//-----------------------------------------------------------------------------

void RoiCounters::get_device_property()
{
    //	Initialize your default values here (if not done with  POGO).
    //------------------------------------------------------------------

    //	Read device properties from database.(Automatic code generation)
    //------------------------------------------------------------------
    Tango::DbData dev_prop;
    dev_prop.push_back(Tango::DbDatum("NbRoiCounters"));
    dev_prop.push_back(Tango::DbDatum("__x"));
    dev_prop.push_back(Tango::DbDatum("__y"));
    dev_prop.push_back(Tango::DbDatum("__width"));
    dev_prop.push_back(Tango::DbDatum("__height"));

    //	Call database and extract values
    //--------------------------------------------
    if(Tango::Util::instance()->_UseDb == true)
        get_db_device()->get_property(dev_prop);
    Tango::DbDatum def_prop, cl_prop;
    RoiCountersClass *ds_class =
    (static_cast<RoiCountersClass *>(get_device_class()));
    int i = -1;

    //	Try to initialize NbRoiCounters from class property
    cl_prop = ds_class->get_class_property(dev_prop[++i].name);
    if(cl_prop.is_empty() == false) cl_prop >> nbRoiCounters;
    else
    {
        //	Try to initialize NbRoiCounters from default device value
        def_prop = ds_class->get_default_device_property(dev_prop[i].name);
        if(def_prop.is_empty() == false) def_prop >> nbRoiCounters;
    }
    //	And try to extract NbRoiCounters value from database
    if(dev_prop[i].is_empty() == false) dev_prop[i] >> nbRoiCounters;

    //	Try to initialize __x from class property
    cl_prop = ds_class->get_class_property(dev_prop[++i].name);
    if(cl_prop.is_empty() == false) cl_prop >> __x;
    else
    {
        //	Try to initialize __x from default device value
        def_prop = ds_class->get_default_device_property(dev_prop[i].name);
        if(def_prop.is_empty() == false) def_prop >> __x;
    }
    //	And try to extract __x value from database
    if(dev_prop[i].is_empty() == false) dev_prop[i] >> __x;

    //	Try to initialize __y from class property
    cl_prop = ds_class->get_class_property(dev_prop[++i].name);
    if(cl_prop.is_empty() == false) cl_prop >> __y;
    else
    {
        //	Try to initialize __y from default device value
        def_prop = ds_class->get_default_device_property(dev_prop[i].name);
        if(def_prop.is_empty() == false) def_prop >> __y;
    }
    //	And try to extract __y value from database
    if(dev_prop[i].is_empty() == false) dev_prop[i] >> __y;

    //	Try to initialize __width from class property
    cl_prop = ds_class->get_class_property(dev_prop[++i].name);
    if(cl_prop.is_empty() == false) cl_prop >> __width;
    else
    {
        //	Try to initialize __width from default device value
        def_prop = ds_class->get_default_device_property(dev_prop[i].name);
        if(def_prop.is_empty() == false) def_prop >> __width;
    }
    //	And try to extract __width value from database
    if(dev_prop[i].is_empty() == false) dev_prop[i] >> __width;

    //	Try to initialize __height from class property
    cl_prop = ds_class->get_class_property(dev_prop[++i].name);
    if(cl_prop.is_empty() == false) cl_prop >> __height;
    else
    {
        //	Try to initialize __height from default device value
        def_prop = ds_class->get_default_device_property(dev_prop[i].name);
        if(def_prop.is_empty() == false) def_prop >> __height;
    }
    //	And try to extract __height value from database
    if(dev_prop[i].is_empty() == false) dev_prop[i] >> __height;



    //	End of Automatic code generation
    //------------------------------------------------------------------
    PropertyHelper::create_property_if_empty(this, dev_prop, "2", "NbRoiCounters");
    vector<string> vecXY(2, "0");
    PropertyHelper::create_property_if_empty(this, dev_prop, vecXY, "__x");
    PropertyHelper::create_property_if_empty(this, dev_prop, vecXY, "__y");
    vector<string> vecWidthHeight(2, "10");
    PropertyHelper::create_property_if_empty(this, dev_prop, vecWidthHeight, "__width");
    PropertyHelper::create_property_if_empty(this, dev_prop, vecWidthHeight, "__height");
}

//+----------------------------------------------------------------------------
//
// method : 		RoiCounters::always_executed_hook()
// 
// description : 	method always executed before any command is executed
//
//-----------------------------------------------------------------------------

void RoiCounters::always_executed_hook()
{
    yat::AutoMutex<> _lock(ControlFactory::instance().get_global_mutex());
    try
    {    
        if(!m_is_device_initialized)//nothing to do ! device need init
            return;
        
        //- get the singleton control objet used to pilot the lima framework
        m_ct = ControlFactory::instance().get_control("RoiCounters");

        dev_state();
    }
    catch(Exception& e)
    {
        ERROR_STREAM << e.getErrMsg() << endl;
        m_status_message << "Initialization Failed : " << e.getErrMsg() << endl;
        //- throw exception
        set_state(Tango::FAULT);
        m_is_device_initialized = false;
        return;
    }
    catch(Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        INFO_STREAM << "Initialization Failed : " << string(df.errors[0].desc) << endl;
        m_status_message << "Initialization Failed : " << string(df.errors[0].desc) << endl;
        m_is_device_initialized = false;
        set_state(Tango::FAULT);
        return;
    }
}
//+----------------------------------------------------------------------------
//
// method : 		RoiCounters::read_attr_hardware
// 
// description : 	Hardware acquisition for attributes.
//
//-----------------------------------------------------------------------------

void RoiCounters::read_attr_hardware(vector<long> &attr_list)
{
    DEBUG_STREAM << "RoiCounters::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
    //	Add your own code here
    try
    {
        if(is_device_initialized())
            read_roi();
    }
    catch(Exception& e)
    {
        ERROR_STREAM << e.getErrMsg() << endl;
        //- throw exception
        Tango::Except::throw_exception(
        static_cast<const char*>("TANGO_DEVICE_ERROR"),
        static_cast<const char*>(e.getErrMsg().c_str()),
        static_cast<const char*>("RoiCounters::read_attr_hardware"));
    }
}
//+----------------------------------------------------------------------------
//
// method : 		RoiCounters::read_version
// 
// description : 	Extract real attribute values for version acquisition result.
//
//-----------------------------------------------------------------------------

void RoiCounters::read_version(Tango::Attribute &attr)
{
    DEBUG_STREAM << "RoiCounters::read_version(Tango::Attribute &attr) entering... " << endl;
    try
    {
        strcpy(*attr_version_read, CURRENT_VERSION);
        attr.set_value(attr_version_read);
    }
    catch(Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
        static_cast<const char*>("TANGO_DEVICE_ERROR"),
        static_cast<const char*>(string(df.errors[0].desc).c_str()),
        static_cast<const char*>("RoiCounters::read_version()"));
    }
}


//+------------------------------------------------------------------
//	RoiCounters::update_roi
//+------------------------------------------------------------------

void RoiCounters::update_roi()
{
    INFO_STREAM << "RoiCounters::update_roi() - [BEGIN]" << endl;

    //	Add your own code to control device here
    yat::AutoMutex<> _lock(ControlFactory::instance().get_global_mutex());
    try
    {
        std::list<SoftOpRoiCounter::RoiNameAndRoi> listNameAndRois;
        SoftOpRoiCounter::RoiNameAndRoi roiNameAndRoi;
        for(int i = 0; i < nbRoiCounters; i++)
        {
            Roi roi(attr_x_array[i], attr_y_array[i], attr_width_array[i], attr_height_array[i]);
            std::stringstream ssRoiName;
            ssRoiName << i;
            roiNameAndRoi = std::make_pair(ssRoiName.str(), roi);
            listNameAndRois.push_back(roiNameAndRoi);
        }

        for(std::list<SoftOpRoiCounter::RoiNameAndRoi>::iterator iter = listNameAndRois.begin();
        iter != listNameAndRois.end();
        iter++)
        {
            INFO_STREAM << "roi " << (*iter).first << "\t : " << (*iter).second << endl;
        }

        if((reinterpret_cast<SoftOpRoiCounter*>(m_soft_operation.m_opt)) != 0)
        {
            (reinterpret_cast<SoftOpRoiCounter*>(m_soft_operation.m_opt))->clearCounterStatus();
            (reinterpret_cast<SoftOpRoiCounter*>(m_soft_operation.m_opt))->updateRois(listNameAndRois);
        }
    }
    catch (ProcessException& p)
    {
        ERROR_STREAM<<"ERROR ProcessException"<<endl;
    }
    catch(Exception& e)
    {
        ERROR_STREAM << e.getErrMsg() << endl;
        //- throw exception
        Tango::Except::throw_exception(
        static_cast<const char*>("TANGO_DEVICE_ERROR"),
        static_cast<const char*>(e.getErrMsg().c_str()),
        static_cast<const char*>("RoiCounters::update_roi"));
    }
    INFO_STREAM << "RoiCounters::update_roi() - [END]" << endl;
}

//+------------------------------------------------------------------
// RoiCounters::read_roi
//+------------------------------------------------------------------

void RoiCounters::read_roi()
{
    DEBUG_STREAM << "RoiCounters::read_roi() - [BEGIN]" << endl;

    //	Add your own code to control device here
    yat::AutoMutex<> _lock(ControlFactory::instance().get_global_mutex());
    try
    {
        if((reinterpret_cast<SoftOpRoiCounter*>(m_soft_operation.m_opt)) != 0)
        {
            int counter_status = (reinterpret_cast<SoftOpRoiCounter*>(m_soft_operation.m_opt))->getCounterStatus();

            if(counter_status < 0)
                return;

            std::list<SoftOpRoiCounter::RoiNameAndResults> roiNameAndResults;
            (reinterpret_cast<SoftOpRoiCounter*>(m_soft_operation.m_opt))->readCounters(counter_status, roiNameAndResults);

            for(std::list<SoftOpRoiCounter::RoiNameAndResults>::iterator iter1 = roiNameAndResults.begin();
            iter1 != roiNameAndResults.end();
            iter1++)
            {
                std::istringstream roiname((*iter1).first);
                int roinum;
                roiname>>roinum;
                for(std::list<Tasks::RoiCounterResult>::iterator iter2 = ((*iter1).second).begin();
                iter2 != ((*iter1).second).end();
                iter2++)
                {
                    INFO_STREAM << "++++++++++++++++++++++++++++++++" << endl;
                    INFO_STREAM << "+++ roi n�: " << roinum << "\t" << endl;
                    INFO_STREAM << "++++++++++++++++++++++++++++++++" << endl;

                    attr_frameNumber_value = (*iter2).frameNumber + 1;
                    attr_sum_array[roinum] = (*iter2).sum;
                    attr_average_array[roinum] = (*iter2).average;
                    attr_std_array[roinum] = (*iter2).std;
                    attr_minValue_array[roinum] = (*iter2).minValue;
                    attr_maxValue_array[roinum] = (*iter2).maxValue;

                    INFO_STREAM << "frameNumber = " << attr_frameNumber_value << endl;
                    INFO_STREAM << "sum         = " << attr_sum_array[roinum] << endl;
                    INFO_STREAM << "average     = " << attr_average_array[roinum] << endl;
                    INFO_STREAM << "std         = " << attr_std_array[roinum] << endl;
                    INFO_STREAM << "minValue    = " << attr_minValue_array[roinum] << endl;
                    INFO_STREAM << "maxValue    = " << attr_maxValue_array[roinum] << endl;
                    INFO_STREAM << "++++++++++++++++++++++++++++++++" << endl;
                }
            }
        }
    }
    catch (ProcessException& p)
    {
        ERROR_STREAM<<"ERROR ProcessException"<<endl;
    }    
    catch(Exception& e)
    {
        ERROR_STREAM << e.getErrMsg() << endl;
        //- throw exception
        Tango::Except::throw_exception(
        static_cast<const char*>("TANGO_DEVICE_ERROR"),
        static_cast<const char*>(e.getErrMsg().c_str()),
        static_cast<const char*>("RoiCounters::read_roi"));
    }
    DEBUG_STREAM << "RoiCounters::read_roi() - [END]" << endl;
}

//-------------------------------------------------------------------
//	RoiCounters::remove_roi
//-------------------------------------------------------------------

void RoiCounters::remove_roi(std::string roiId)
{
    INFO_STREAM << "RoiCounters::remove_roi(" << roiId << ")" << endl;
    //	Add your own code to control device here
    yat::AutoMutex<> _lock(ControlFactory::instance().get_global_mutex());
    try
    {
        if((reinterpret_cast<SoftOpRoiCounter*>(m_soft_operation.m_opt)) != 0)
        {
            std::list<std::string> listRois;
            listRois.push_back(roiId);
            (reinterpret_cast<SoftOpRoiCounter*>(m_soft_operation.m_opt))->removeRois(listRois);
        }
    }
    catch(Exception& e)
    {
        ERROR_STREAM << e.getErrMsg() << endl;
        //- throw exception
        Tango::Except::throw_exception(
        static_cast<const char*>("TANGO_DEVICE_ERROR"),
        static_cast<const char*>(e.getErrMsg().c_str()),
        static_cast<const char*>("RoiCounters::remove_roi"));
    }
}

//-------------------------------------------------------------------
//	RoiCounters::create_dynamic_attribute
//-------------------------------------------------------------------

template <typename T>
void RoiCounters::create_dynamic_attribute(std::string name, int type, Tango::AttrDataFormat format, Tango::AttrWriteType writetype, T* user_data)
{
    DEBUG_STREAM << "RoiCounters::create_dynamic_attribute() - [BEGIN]" << endl;
    INFO_STREAM << "\t- Create dynamic attribute [" << name << "]" << endl;
    ////////////////////////////////////////////////////////////////////////////////////////
    yat4tango::DynamicAttributeInfo dai;
    dai.dev = this;
    //- initialize the associated data 
    *user_data = 0;
    //- specify the dyn. attr.  name
    dai.tai.name = name;
    //- associate the dyn. attr. with its data (see read_callback for usage)
    dai.set_user_data(user_data);
    //- describe the dynamic attr we want...
    dai.tai.unit = " ";
    dai.tai.data_type = type;
    dai.tai.data_format = format;
    dai.tai.writable = writetype;
    dai.tai.disp_level = Tango::OPERATOR;
    //	//- cleanup tango db option: cleanup tango db when removing this dyn. attr. (i.e. erase its properties fom db)
    //	dai.cdb = true;

    if(dai.tai.writable == Tango::READ)
    {
        //- instanciate the read callback (called when the dyn. attr. is read)
        dai.rcb = yat4tango::DynamicAttributeReadCallback::instanciate(*this, &RoiCounters::read_stats_callback);
    }
    else if(dai.tai.writable == Tango::READ_WRITE)//don't care about read_with_write and others
    {
        //- instanciate the read callback (called when the dyn. attr. is read)
        dai.rcb = yat4tango::DynamicAttributeReadCallback::instanciate(*this, &RoiCounters::read_rois_callback);

        //- instanciate the write callback (called when the dyn. attr. is written)
        dai.wcb = yat4tango::DynamicAttributeWriteCallback::instanciate(*this, &RoiCounters::write_rois_callback);
    }

    //- add the dyn. attr. to the device
    m_dim.dynamic_attributes_manager().add_attribute(dai);
    DEBUG_STREAM << "RoiCounters::create_dynamic_attribute() - [END]" << endl;
}

//-------------------------------------------------------------------
//	RoiCounters::create_all_dynamic_attributes
//-------------------------------------------------------------------

bool RoiCounters::create_all_dynamic_attributes(void)
{
    DEBUG_STREAM << "RoiCounters::create_all_dynamic_attributes() - [BEGIN]" << endl;
    INFO_STREAM << "Create all dynamic attributes :" << endl;
    if(nbRoiCounters > MAX_NB_ROICOUNTERS)
    {
        m_status_message.str("");
        m_status_message  <<"Failed to instanciate dynamic attributes  - "
                <<"nbRoiCounters property must not exceed : "
                << MAX_NB_ROICOUNTERS
                <<std::endl;
        ERROR_STREAM << m_status_message.str() << std::endl;
        set_state(Tango::FAULT);
        set_status(m_status_message.str());
        return false;        
    }
    
    //- add some dynamic attributes
    try
    {
        std::stringstream ssName;
        ssName.str("");
        ssName << "frameNumber";
        create_dynamic_attribute<Tango::DevULong>(ssName.str(), Tango::DEV_ULONG, Tango::SCALAR, Tango::READ, &attr_frameNumber_value);

        for(size_t i = 0; i < nbRoiCounters; ++i)
        {
            ssName.str("");
            ssName << "x" << i;
            create_dynamic_attribute<Tango::DevULong>(ssName.str(), Tango::DEV_ULONG, Tango::SCALAR, Tango::READ_WRITE, &attr_x_array[i]);

            ssName.str("");
            ssName << "y" << i;
            create_dynamic_attribute<Tango::DevULong>(ssName.str(), Tango::DEV_ULONG, Tango::SCALAR, Tango::READ_WRITE, &attr_y_array[i]);

            ssName.str("");
            ssName << "width" << i;
            create_dynamic_attribute<Tango::DevULong>(ssName.str(), Tango::DEV_ULONG, Tango::SCALAR, Tango::READ_WRITE, &attr_width_array[i]);

            ssName.str("");
            ssName << "height" << i;
            create_dynamic_attribute<Tango::DevULong>(ssName.str(), Tango::DEV_ULONG, Tango::SCALAR, Tango::READ_WRITE, &attr_height_array[i]);

            ssName.str("");
            ssName << "sum" << i;
            create_dynamic_attribute<Tango::DevDouble>(ssName.str(), Tango::DEV_DOUBLE, Tango::SCALAR, Tango::READ, &attr_sum_array[i]);

            ssName.str("");
            ssName << "average" << i;
            create_dynamic_attribute<Tango::DevDouble>(ssName.str(), Tango::DEV_DOUBLE, Tango::SCALAR, Tango::READ, &attr_average_array[i]);

            ssName.str("");
            ssName << "std" << i;
            create_dynamic_attribute<Tango::DevDouble>(ssName.str(), Tango::DEV_DOUBLE, Tango::SCALAR, Tango::READ, &attr_std_array[i]);

            ssName.str("");
            ssName << "minValue" << i;
            create_dynamic_attribute<Tango::DevDouble>(ssName.str(), Tango::DEV_DOUBLE, Tango::SCALAR, Tango::READ, &attr_minValue_array[i]);

            ssName.str("");
            ssName << "maxValue" << i;
            create_dynamic_attribute<Tango::DevDouble>(ssName.str(), Tango::DEV_DOUBLE, Tango::SCALAR, Tango::READ, &attr_maxValue_array[i]);
        }
    }
    catch(Tango::DevFailed& df)
    {
        std::string err("failed to instanciate dynamic attributes  - Tango exception caught - see log attribute for details");
        ERROR_STREAM << err << std::endl;
        ERROR_STREAM << df << std::endl;
        this->set_state(Tango::FAULT);
        this->set_status(err);
        return false;
    }
    catch(...)
    {
        this->set_state(Tango::FAULT);
        this->set_status("failed to instanciate dynamic attributes - unknown exception caught");
        return false;
    }
    DEBUG_STREAM << "RoiCounters::create_all_dynamic_attributes() - [END]" << endl;
    return true;
}

//----------------------------------------------------------------------------------------
// DYN. ATTRS. READ CALLBACK
//----------------------------------------------------------------------------------------

void RoiCounters::read_stats_callback(yat4tango::DynamicAttributeReadCallbackData& cbd)
{
    DEBUG_STREAM << "RoiCounters::read_stats_callback(yat4tango::DynamicAttributeReadCallbackData& cbd) - [BEGIN]" << endl;
    yat::AutoMutex<> _lock(ControlFactory::instance().get_global_mutex());
    //- be sure the pointer to the dyn. attr. is valid
    if(!cbd.dya)
    {
        THROW_DEVFAILED("INTERNAL_ERROR", "unexpected NULL pointer to dynamic attribute", "DynamicInterface::read_stats_callback");
    }

    try
    {
        void* val;
        std::string myAttributeName = cbd.dya->get_name();

        if(
        (myAttributeName.find("sum") != std::string::npos) ||
        (myAttributeName.find("average") != std::string::npos) ||
        (myAttributeName.find("std") != std::string::npos) ||
        (myAttributeName.find("minValue") != std::string::npos) ||
        (myAttributeName.find("maxValue") != std::string::npos))
        {
            val = (Tango::DevDouble*)cbd.dya->get_user_data<Tango::DevDouble>();

            //- set the attribute value
            cbd.tga->set_value((Tango::DevDouble*)val);
        }
        else if(myAttributeName.find("frameNumber") != std::string::npos)
        {
            val = (Tango::DevULong*)cbd.dya->get_user_data<Tango::DevULong>();

            //- set the attribute value
            cbd.tga->set_value((Tango::DevULong*)val);
        }
    }
    catch(Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
        static_cast<const char*>("TANGO_DEVICE_ERROR"),
        static_cast<const char*>(string(df.errors[0].desc).c_str()),
        static_cast<const char*>("RoiCounters::read_stats_callback"));
    }
    DEBUG_STREAM << "RoiCounters::read_stats_callback(yat4tango::DynamicAttributeReadCallbackData& cbd) - [END]" << endl;
}

//----------------------------------------------------------------------------------------
// DYN. ATTRS. READ CALLBACK
//----------------------------------------------------------------------------------------

void RoiCounters::read_rois_callback(yat4tango::DynamicAttributeReadCallbackData& cbd)
{
    DEBUG_STREAM << "RoiCounters::read_rois_callback(yat4tango::DynamicAttributeReadCallbackData& cbd) - [BEGIN]" << endl;
    yat::AutoMutex<> _lock(ControlFactory::instance().get_global_mutex());
    try
    {
        void* val;
        std::string attrName = cbd.tga->get_name();

        //--------------------------------------------------------------
        if(std::size_t pos = attrName.find("x") != std::string::npos)
        {
            std::string strIndex = attrName.substr(1);
            int attrIndex;
            std::istringstream(strIndex) >> attrIndex;
            val = (Tango::DevULong *)&attr_x_array[attrIndex];
        }
        else if(std::size_t pos = attrName.find("y") != std::string::npos)
        {
            std::string strIndex = attrName.substr(1);
            int attrIndex;
            std::istringstream(strIndex) >> attrIndex;
            val = (Tango::DevULong *)&attr_y_array[attrIndex];
        }
        else if(std::size_t pos = attrName.find("width") != std::string::npos)
        {
            std::string strIndex = attrName.substr(5);
            int attrIndex;
            std::istringstream(strIndex) >> attrIndex;
            val = (Tango::DevULong *)&attr_width_array[attrIndex];
        }
        else if(std::size_t pos = attrName.find("height") != std::string::npos)
        {
            std::string strIndex = attrName.substr(6);
            int attrIndex;
            std::istringstream(strIndex) >> attrIndex;
            val = (Tango::DevULong *)&attr_height_array[attrIndex];
        }

        cbd.tga->set_value((Tango::DevULong *)val);
    }
    catch(Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
        static_cast<const char*>("TANGO_DEVICE_ERROR"),
        static_cast<const char*>(string(df.errors[0].desc).c_str()),
        static_cast<const char*>("RoiCounters::read_rois_callback"));
    }
    DEBUG_STREAM << "RoiCounters::read_rois_callback(yat4tango::DynamicAttributeReadCallbackData& cbd) - [END]" << endl;
}

//----------------------------------------------------------------------------------------
// DYN. ATTRS. WRITE CALLBACK
//----------------------------------------------------------------------------------------

void RoiCounters::write_rois_callback(yat4tango::DynamicAttributeWriteCallbackData& cbd)
{
    INFO_STREAM << "RoiCounters::write_rois_callback(yat4tango::DynamicAttributeWriteCallbackData& cbd) - [BEGIN]" << endl;
    yat::AutoMutex<> _lock(ControlFactory::instance().get_global_mutex());
    try
    {
        std::string attrName = cbd.tga->get_name();
        int attrIndex = -1;
        void*  val;
        //- log	
        INFO_STREAM << "Write on attribute  : " << attrName << std::endl;

        //-------------------------------------------------------------- 
        if(attrName.find("x") != std::string::npos)
        {
            std::string strIndex = attrName.substr(1);
            std::istringstream(strIndex) >> attrIndex;
            val = (Tango::DevULong*)(&attr_x_array[attrIndex]);
        }
        else if(attrName.find("y") != std::string::npos)
        {
            std::string strIndex = attrName.substr(1);
            std::istringstream(strIndex) >> attrIndex;
            val = (Tango::DevULong*)(&attr_y_array[attrIndex]);
        }
        else if(attrName.find("width") != std::string::npos)
        {
            std::string strIndex = attrName.substr(5);
            std::istringstream(strIndex) >> attrIndex;
            val = (Tango::DevULong*)(&attr_width_array[attrIndex]);
        }
        else if(attrName.find("height") != std::string::npos)
        {
            std::string strIndex = attrName.substr(6);
            std::istringstream(strIndex) >> attrIndex;
            val = (Tango::DevULong*)(&attr_height_array[attrIndex]);
        }

        //- get the attribute value
        cbd.tga->get_write_value(*((Tango::DevULong*)val));

        //- update rois in the processLib  : SoftOpRoiCounter        
        update_roi();

        //memorize the attribute in its position in the vector property
        __x.at(attrIndex)       = attr_x_array[attrIndex];
        __y.at(attrIndex)       = attr_y_array[attrIndex];
        __width.at(attrIndex)   = attr_width_array[attrIndex];
        __height.at(attrIndex)  = attr_height_array[attrIndex];        
        
        PropertyHelper::set_property(this, "__x", __x);
        PropertyHelper::set_property(this, "__y", __y);
        PropertyHelper::set_property(this, "__width", __width);
        PropertyHelper::set_property(this, "__height", __height);
    }
    catch(Exception& e)
    {
        ERROR_STREAM << e.getErrMsg() << endl;
        //- throw exception
        Tango::Except::throw_exception(
        static_cast<const char*>("TANGO_DEVICE_ERROR"),
        static_cast<const char*>(e.getErrMsg().c_str()),
        static_cast<const char*>("RoiCounters::write_rois_callback"));
    }
    catch(Tango::DevFailed& df)
    {
        ERROR_STREAM << df << endl;
        //- rethrow exception
        Tango::Except::re_throw_exception(df,
        static_cast<const char*>("TANGO_DEVICE_ERROR"),
        static_cast<const char*>(string(df.errors[0].desc).c_str()),
        static_cast<const char*>("RoiCounters::write_rois_callback"));
    }
    INFO_STREAM << "RoiCounters::write_rois_callback(yat4tango::DynamicAttributeWriteCallbackData& cbd) - [END]" << endl;
}

//+------------------------------------------------------------------
/**
 *	method:	RoiCounters::dev_state
 *
 *	description:	method to execute "State"
 *	This command gets the device state (stored in its <i>device_state</i> data member) and returns it to the caller.
 *
 * @return	State Code
 *
 */
//+------------------------------------------------------------------

Tango::DevState RoiCounters::dev_state()
{
    Tango::DevState argout = DeviceImpl::dev_state();
    DEBUG_STREAM << "RoiCounters::dev_state(): entering... !" << endl;
    //    Add your own code to control device here	
    stringstream DeviceStatus;
    DeviceStatus << "";
    Tango::DevState DeviceState = Tango::STANDBY;
    if(!m_is_device_initialized)
    {
        DeviceState = Tango::FAULT;
        DeviceStatus << m_status_message.str();
    }
    else
    {
        // state & status are retrieved from Factory, Factory is updated by Generic device
        DeviceState = ControlFactory::instance().get_state();
        DeviceStatus << ControlFactory::instance().get_status();
    }

    set_state(DeviceState);
    set_status(DeviceStatus.str());

    argout = DeviceState;
    return argout;
}






} //	namespace
