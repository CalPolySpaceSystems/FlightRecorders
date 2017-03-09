function formAlter( handles , setting )


switch setting
    
    case 'serialOn'
        
        set(handles.btn_toggleserial,'string','Close Serial');
        set(handles.btn_recordserial,'enable','on');
        
        set(handles.txt_playSerial,'ForegroundColor',[0,0.8,0]);
        
        set(handles.tbx_serialport,'enable','inactive');
        set(handles.ppm_baud,'enable','inactive');
        set(handles.rbn_parse,'enable','off');
        set(handles.rbn_setLength,'enable','off');
        set(handles.btn_parseSettings,'enable','off');
        
        if get(handles.rbn_parse,'Value')
            set(handles.tbx_bitstream,'enable','off');
            set(handles.txt_length,'enable', 'off');
        else
            set(handles.tbx_bitstream,'enable','inactive');
        end
            
        set(handles.btn_toggleGPS,'enable','on');
        set(handles.btn_kmlConvert,'enable','off');
        
    case 'serialOff'
        
        set(handles.btn_toggleserial,'string','Open Serial');
        set(handles.btn_recordserial,'enable','off'); 
        
        set(handles.txt_playSerial,'ForegroundColor',[0.7,0.7,0.7]);
        
        set(handles.tbx_serialport,'enable','on');             
        set(handles.ppm_baud,'enable','on');
        set(handles.rbn_parse,'enable','on');
        set(handles.rbn_setLength,'enable','on');
        
        
        if get(handles.rbn_parse,'Value')
            set(handles.tbx_bitstream,'enable','off');
            set(handles.txt_length,'enable', 'off');
            set(handles.btn_parseSettings,'enable','on');
        else
            set(handles.tbx_bitstream,'enable','on');
        end
 
        set(handles.btn_toggleGPS,'enable','off');    
        set(handles.btn_kmlConvert,'enable','on');
        
    case 'serialRecOn'
        
        set(handles.txt_recSerial,'ForegroundColor',[0.8,0,0]);
    
        set(handles.btn_recordserial,'string','Stop Recording');

        set(handles.btn_toggleserial,'Value',0);
        set(handles.btn_toggleserial,'enable','off');
           
    case 'serialRecOff'
        
        set(handles.txt_recSerial,'ForegroundColor',[0.7,0.7,0.7]);
    
        set(handles.btn_recordserial,'string','Record Serial');

        set(handles.btn_toggleserial,'enable','on');
        
    case 'gpsOn'
        
        set(handles.btn_toggleGPS,'String','Stop Getting GPS')
 
        set(handles.btn_toggleserial,'Value',0);
        set(handles.btn_toggleserial,'enable','off');
        
        set(handles.txt_playGPS,'ForegroundColor',[0,0.8,0]);
    
    case 'gpsOff'
        
        set(handles.btn_toggleGPS,'String','Get GPS')

        set(handles.btn_toggleserial,'enable','on');
        
        set(handles.txt_playGPS,'ForegroundColor',[0.7,0.7,0.7]);
        
end

end

