function varargout = kmlConverter(varargin)
% KMLCONVERTER MATLAB code for kmlConverter.fig
%      KMLCONVERTER, by itself, creates a new KMLCONVERTER or raises the existing
%      singleton*.
%
%      H = KMLCONVERTER returns the handle to a new KMLCONVERTER or the handle to
%      the existing singleton*.
%
%      KMLCONVERTER('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in KMLCONVERTER.M with the given input arguments.
%
%      KMLCONVERTER('Property','Value',...) creates a new KMLCONVERTER or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before kmlConverter_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to kmlConverter_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help kmlConverter

% Last Modified by GUIDE v2.5 28-Feb-2017 12:51:03

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @kmlConverter_OpeningFcn, ...
                   'gui_OutputFcn',  @kmlConverter_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT

%% FORM DIRECT FUNCTIONS

% --- Executes just before kmlConverter is made visible.
function kmlConverter_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to kmlConverter (see VARARGIN)

global warnAudio
warnAudio = audioplayer([sin(1:.6:400), sin(1:.7:400), sin(1:.4:400)], 22050);

% Choose default command line output for kmlConverter
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% --- Outputs from this function are returned to the command line.
function varargout = kmlConverter_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

%% FILE SELECTION

% --- Browse for log file
function btn_browse_Callback(hObject, eventdata, handles)

global fullPath

[fileName,pathName] = uigetfile('*.txt','Select a serial log to create a .kml file.');

if fileName
    
    fullPath = strcat([pathName fileName]);
    set(handles.tbx_filePath,'string',fullPath);
    
end

%% KML FILE OPTIONS

% --- Opacity slider and text box
function sld_lineOpacity_Callback(hObject, eventdata, handles)

set(handles.tbx_lineOpacity,'string',string(100 * get(hObject,'Value')));

function tbx_lineOpacity_Callback(hObject, eventdata, handles)

set(handles.sld_lineOpacity,'Value',(.01 * get(hObject,'String')));

% --- Width slider and text box
function sld_lineWidth_Callback(hObject, eventdata, handles)

set(handles.tbx_lineWidth,'string',string(100 * get(hObject,'Value')));

function tbx_lineWidth_Callback(hObject, eventdata, handles)

set(handles.sld_lineWidth,'Value',(.01 * get(hObject,'String')));

%% EXECUTION BUTTONS

% --- Assembles the kml file
function btn_enter_Callback(hObject, eventdata, handles)

global warnAudio

%% Gather info from form
try

    % Get target path and open
    fullPath = get(handles.tbx_filePath,'String');
    fID = fopen(fullPath);
    
    % Get kml file name
    kmlFile = strcat(['logs\' (get(handles.tbx_kmlName,'String'))]);
    
    % Choose color
    switch get(handles.ppm_lineColor,'Value')
         
        case 1
            lineColor = 'none';
            
        case 2
            lineColor = [1 0 0];
            
        case 3
            lineColor = [1 1 0];
            
        case 4
            lineColor = [0 1 0];
            
        case 5
            lineColor = [0 1 1];
            
        case 6
            lineColor = [0 0 1];
            
        case 7
            lineColor = [1 0 1];
            
        case 8
            lineColor = [0 0 0];
            
        case 9
            lineColor = [1 1 1];
            
        case 10
            lineColor = [.5 .5 .5];
            
    end
    
    % Get opacity
    opacity = get(handles.sld_lineOpacity, 'Value');
    
    % Get line width 
    width = 100*(get(handles.sld_lineWidth, 'Value'));
   
    % Get description
    desc = get(handles.tbx_description,'String');
    
catch
    % Warning Dialog
    play(warnAudio);
    errordlg('Error setting .kml values.','Error','modal');
end
    
%% Get data from log, determine delimiter
try
    % Read strings from log
    strings = textscan(fID,'%s','Delimiter','\n');
    strings = strings{1,1};
   
    % Get delimiter
    delimTest = char(strings(1,1));
    delimiter = 'a';
    
    for i = 2:30
       
        if isstrprop(delimTest(1,i),'alphanum') == false
            
            if ((delimTest(1,i) == '-') == false) && ((delimTest(1,i) == '.') == false)
                
                % Must be a delimiter
                delimiter = delimTest(1,i);
                break
                
            end
            
        end
        
    end
    
catch
end
    
    if delimiter == 'a'
        
        % Warning Dialog
        play(warnAudio);
        errordlg('Could not find delimiter.','Error','modal');
        fclose(fID);
        return
        
    end
    
%% Assemble array of points for file    
try

    j = 0;
    while (1)
    
        j = j+1;    
        
        GPSVals = textscan(strings{j,1},'%s','Delimiter',',');
        
        if str2double(GPSVals{1,1}{6,1}) == 1
            
            kml(j,1) = str2double(GPSVals{1,1}{4,1});
            kml(j,2) = str2double(GPSVals{1,1}{5,1});
            kml(j,3) = str2double(GPSVals{1,1}{7,1});
            
        end

    end
    
catch
    
    % Warning Dialog
    play(warnAudio);
    errordlg('Error writing .kml file','Error','modal');
    fclose(fID);
    return
    
end

%% Actually write the file

try
    
    kmlwriteline   (kmlFile,kml(:,1),kml(:,2),kml(:,3),...
                    'Color',lineColor,...
                    'Alpha',opacity,...
                    'LineWidth',width,...
                    'Description',desc,...
                    'AltitudeMode','relativeToSeaLevel');
 
    % Warning Sound
    play(warnAudio);
    msgbox('File succesfully written.',''); 
                
catch
    
    % Warning Dialog
    play(warnAudio);
    errordlg('Error writing .kml file','Error','modal');
    
end
            
fclose(fID);
            
% --- Cancels and closes the form
function btn_cancel_Callback(hObject, eventdata, handles)

global warnAudio

%Set default
exitConfirm = false;

% Get dialog result
exitConfirm = dialogWarnYN('Are you sure you wish to close?','Close',warnAudio);

if exitConfirm == true
    delete(hObject);
end
