function [ lengthPass, checkSumPass, syntaxPass] = checkInput(inString,  dataLength, delimiter , CSDelimiter, checkSum)
% checkInput Checks if an input string matches its length and checksum
%{ 
        Format of input string: $######,#######,######;LENGTH,CHECKSUM*
 with "$", ",", ";", and "*" cooresponding to start characters, delimiters
 checksum delimiters, and end characters (although only delimiters and
 checksum delimiters matter for this function.)
%}
    
    % Initial conditions
    lengthPass = false;
    checkSumPass = false;
    syntaxPass = true;

    try   
        cValuesIn = (strsplit(inString,CSDelimiter));   
        cValuesIn = (strsplit(string(cValuesIn(1,2)),delimiter));
    
        % Check Length
        if strcmp(dataLength,cValuesIn{1,1})
    
            lengthPass = true;

        end
        
        % Get check sum as computed by sender
        cSumIn = (dec2hex(str2num(cValuesIn{1,2})));
        
        if strcmpi(checkSum,cValuesIn{1,2})
    
            checkSumPass = true;
                                
        end
        
    catch 
        
        % Other error with syntax (or with code)
        syntaxPass = false;
                           
    end

end

