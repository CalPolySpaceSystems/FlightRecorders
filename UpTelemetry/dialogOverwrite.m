function dialogResult = dialogOverwrite(titleString,warnAudio)

% Notice sound
play(warnAudio);    

% Dialog
d = dialog('Position',[300 300 240 100],'Name',titleString,'WindowStyle','Normal');

    txt = uicontrol('Parent',d,...
               'Style','text',...
               'Position',[20 45 210 40],...
               'String','A file with this name already exists, Do you wish to overwrite it?');

    btnyes = uicontrol('Parent',d,...
               'Position',[45 20 70 25],...
               'String','Yes',...
               'Callback',@yesSelectionCallback);
           
    btnno = uicontrol('Parent',d,...
               'Position',[125 20 70 25],...
               'String','No',...
               'Callback',@noSelectionCallback);

        res = false
        
    function yesSelectionCallback(src,event);
        res = true
        delete(gcf);
    end

    function noSelectionCallback(src,event);
        res = false
        delete(gcf);
    end

    dialogResult = res;

end
