function [consoleString] = consoleUpdate(newString, targetHandle, maxLines )
% consoleUpdate:    Updates target edit text control with new lines of text
%                   

if isempty(get(targetHandle,'string')) 
    currentString = newString;
else
    currentString = strvcat(get(targetHandle,'string'),newString);
end
    
if size(currentString,1) > maxLines
    
    %Preallocate (for speed)
    stringLength = size(currentString,2);
    consoleString = char(maxLines,stringLength);

    %Shift all lines up
    for i = 1:maxLines
        for j = 1:stringLength
            consoleString(i,j) = currentString(i+1,j);
        end
    end
    
else
    consoleString = currentString;
end

set(targetHandle,'string',consoleString);
drawnow;

end


