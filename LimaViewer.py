#!/usr/bin/env python
# -*- coding:utf-8 -*- 


##############################################################################
## license :
##============================================================================
##
## File :        LimaViewer.py
## 
## Project :     LimaViewer
##
## This file is part of Tango device class.
## 
## Tango is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## Tango is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with Tango.  If not, see <http://www.gnu.org/licenses/>.
## 
##
## $Author :      vincent.bussieres$
##
## $Revision :    $
##
## $Date :        $
##
## $HeadUrl :     $
##============================================================================
##            This file is generated by POGO
##    (Program Obviously used to Generate tango Object)
##
##        (c) - Software Engineering Group - ESRF
##############################################################################

"""This class allows user to test LimaCCDs device server.
"""

__all__ = ["LimaViewer", "LimaViewerClass", "main"]

__docformat__ = 'restructuredtext'

import PyTango
import sys
# Add additional import
#----- PROTECTED REGION ID(LimaViewer.additionnal_import) ENABLED START -----#
import numpy
import time
#----- PROTECTED REGION END -----#  //  LimaViewer.additionnal_import

## Device States Description
## No states for this device

class LimaViewer (PyTango.Device_4Impl):

    #--------- Add you global variables here --------------------------
    #----- PROTECTED REGION ID(LimaViewer.global_variables) ENABLED START -----#
    #attr_Image_ccd_read = []
    attr_Exposure_Time = 0.01
    attr_Nb_Frame = 0
    #----- PROTECTED REGION END -----#  //  LimaViewer.global_variables

    def __init__(self,cl, name):
        PyTango.Device_4Impl.__init__(self,cl,name)
        self.debug_stream("In __init__()")
        LimaViewer.init_device(self)
        #----- PROTECTED REGION ID(LimaViewer.__init__) ENABLED START -----#
            self.ImageType2NumpyType = {
                'Bpp8' : numpy.uint8,
                'Bpp8S' : numpy.int8,
                'Bpp10' : numpy.uint16,
                'Bpp10S' : numpy.int16,
                'Bpp12' : numpy.uint16,
                'Bpp12S' : numpy.int16,
                'Bpp14' : numpy.uint16,
                'Bpp14S' : numpy.int16,
                'Bpp16' : numpy.uint16,
                'Bpp16S' : numpy.int16,
                'Bpp32' :  numpy.uint32,
                'Bpp32S' : numpy.int32,
                'Bpp32F' : numpy.float32,
            }
        #----- PROTECTED REGION END -----#  //  LimaViewer.__init__
        
    def delete_device(self):
        self.debug_stream("In delete_device()")
        #----- PROTECTED REGION ID(LimaViewer.delete_device) ENABLED START -----#
        self.DevCcd.StopAcq()
        #----- PROTECTED REGION END -----#  //  LimaViewer.delete_device

    def init_device(self):
        self.debug_stream("In init_device()")
        self.get_device_properties(self.get_device_class())
        self.attr_Image_ccd_read = [[0]]
        #----- PROTECTED REGION ID(LimaViewer.init_device) ENABLED START -----#
        self.set_state(PyTango.DevState.ON)
        self.get_device_properties(self.get_device_class())  
       
        self.DevCcd = PyTango.DeviceProxy(self.Dev_Ccd_name)
        self.shape_ccd = self.DevCcd.read_attribute('image_sizes').value[::-1]
        self.image_type = self.DevCcd.read_attribute('image_type').value
        if self.image_type not in self.ImageType2NumpyType :
            self.set_state(PyTango.DevState.FAULT)
            self.set_status("Format "+self.image_type+" unsupported")
        self.DevCcd.write_attribute('acq_expo_time',self.attr_Exposure_Time)
        self.DevCcd.write_attribute('acq_nb_frames',self.attr_Nb_Frame)
    
        self.DevCcd.prepareAcq()
        self.DevCcd.StartAcq()
        self.data = []
        #----- PROTECTED REGION END -----#  //  LimaViewer.init_device

    def always_executed_hook(self):
        self.debug_stream("In always_excuted_hook()")
        #----- PROTECTED REGION ID(LimaViewer.always_executed_hook) ENABLED START -----#
        
        #----- PROTECTED REGION END -----#  //  LimaViewer.always_executed_hook

    #-----------------------------------------------------------------------------
    #    LimaViewer read/write attribute methods
    #-----------------------------------------------------------------------------
    
    def write_Exposure_time(self, attr):
        self.debug_stream("In write_Exposure_time()")
        data=attr.get_write_value()
        #----- PROTECTED REGION ID(LimaViewer.Exposure_time_write) ENABLED START -----#
        self.DevCcd.StopAcq()
        try:
            self.DevCcd.write_attribute('acq_expo_time',data)
        except:
            self.debug_stream("Exposure time out of range")
        self.DevCcd.prepareAcq()
        self.DevCcd.StartAcq()
        #----- PROTECTED REGION END -----#  //  LimaViewer.Exposure_time_write
        
    def write_Number_of_frames(self, attr):
        self.debug_stream("In write_Number_of_frames()")
        data=attr.get_write_value()
        #----- PROTECTED REGION ID(LimaViewer.Number_of_frames_write) ENABLED START -----#
        self.DevCcd.StopAcq()
        self.DevCcd.acq_nb_frames = int(data)
        self.DevCcd.prepareAcq()
        self.DevCcd.StartAcq()
        #----- PROTECTED REGION END -----#  //  LimaViewer.Number_of_frames_write
        
    def read_Image_ccd(self, attr):
        self.debug_stream("In read_Image_ccd()")
        #----- PROTECTED REGION ID(LimaViewer.Image_ccd_read) ENABLED START -----#
        if self.get_state() != PyTango.DevState.FAULT:
            data = self.DevCcd.getImage(-1).data
            self.data = data
            numpyType = self.ImageType2NumpyType[self.image_type]
            self.attr_Image_ccd_read = numpy.frombuffer(data, dtype=numpyType).reshape(self.shape_ccd[:2])
        
        attr.set_value(self.attr_Image_ccd_read,self.shape_ccd[1], self.shape_ccd[0])
        
        #----- PROTECTED REGION END -----#  //  LimaViewer.Image_ccd_read
        
    
    
        #----- PROTECTED REGION ID(LimaViewer.initialize_dynamic_attributes) ENABLED START -----#
        
        #----- PROTECTED REGION END -----#  //  LimaViewer.initialize_dynamic_attributes
            
    def read_attr_hardware(self, data):
        self.debug_stream("In read_attr_hardware()")
        #----- PROTECTED REGION ID(LimaViewer.read_attr_hardware) ENABLED START -----#
        
        #----- PROTECTED REGION END -----#  //  LimaViewer.read_attr_hardware


    #-----------------------------------------------------------------------------
    #    LimaViewer command methods
    #-----------------------------------------------------------------------------
    

class LimaViewerClass(PyTango.DeviceClass):
    #--------- Add you global class variables here --------------------------
    #----- PROTECTED REGION ID(LimaViewer.global_class_variables) ENABLED START -----#
    
    #----- PROTECTED REGION END -----#  //  LimaViewer.global_class_variables

    def dyn_attr(self, dev_list):
        """Invoked to create dynamic attributes for the given devices.
        Default implementation calls
        :meth:`LimaViewer.initialize_dynamic_attributes` for each device
    
        :param dev_list: list of devices
        :type dev_list: :class:`PyTango.DeviceImpl`"""
    
        # for dev in dev_list:
            # try:
                # dev.initialize_dynamic_attributes()
            # except:
                # import traceback
                # dev.warn_stream("Failed to initialize dynamic attributes")
                # dev.debug_stream("Details: " + traceback.format_exc())
        #----- PROTECTED REGION ID(LimaViewer.dyn_attr) ENABLED START -----#
        
        #----- PROTECTED REGION END -----#  //  LimaViewer.dyn_attr

    #    Class Properties
    class_property_list = {
        }


    #    Device Properties
    device_property_list = {
        'Dev_Ccd_name':
            [PyTango.DevString,
            "Nom device CCD",
            [ "lima/limaccds/dev0" ] ],
        }


    #    Command definitions
    cmd_list = {
        }


    #    Attribute definitions
    attr_list = {
        'Exposure_time':
            [[PyTango.DevDouble,
            PyTango.SCALAR,
            PyTango.WRITE]],
        'Number_of_frames':
            [[PyTango.DevDouble,
            PyTango.SCALAR,
            PyTango.WRITE]],
        'Image_ccd':
            [[PyTango.DevUShort,
            PyTango.IMAGE,
            PyTango.READ, 4096, 4096],
            {
                'Polling period': "100",
            } ],
        }


def main():
    try:
        py = PyTango.Util(sys.argv)
        py.add_class(LimaViewerClass,LimaViewer,'LimaViewer')

        U = PyTango.Util.instance()
        U.server_init()
        U.server_run()

    except PyTango.DevFailed,e:
        print '-------> Received a DevFailed exception:',e
    except Exception,e:
        print '-------> An unforeseen exception occured....',e

if __name__ == '__main__':
    main()
