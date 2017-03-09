function dialogResult = dialogWarnYN(dialogString,titleString,warnAudio)

% Notice sound
play(warnAudio);    

% Dialog
d = dialog('Position',[300 300 240 100],'Name',titleString);


    txt = uicontrol('Parent',d,...
               'Style','text',...
               'Position',[20 45 210 40],...
               'String',dialogString);

    btnyes = uicontrol('Parent',d,...
               'Position',[45 20 70 25],...
               'String','Yes',...
               'Callback',@yesSelectionCallback);
           
    btnno = uicontrol('Parent',d,...
               'Position',[125 20 70 25],...
               'String','No',...
               'Callback',@noSelectionCallback);

    res = false;
    uiwait(d);
        
    function yesSelectionCallback(btnyes,event);
        res = true;
        delete(gcf);
    end

    function noSelectionCallback(btnno,event);
        res = false;
        delete(gcf);
    end

    dialogResult = res;

end