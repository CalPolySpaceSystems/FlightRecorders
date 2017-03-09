function [checkSum] = checkSum(dataString,dataLength,checkMode)
%checkSum Performs a selected checksum algorithm on the dataString

    switch checkMode
        
        case 'xor'
            
            % Put in char array for easy access
            csChar = unicode2native(char(dataString));
            currentCS = 0;
                        
            for i = 1: dataLength
                           
                currentCS = bitxor(currentCS,csChar(1,i));
                            
            end 
                        
            % Save values of checksum and length
            checkSum = dec2hex(currentCS);
            
        otherwise
            
            % Throw error if mode is not chosen
            msg = ('Error: checkMode not supported.');  
            error(msg);
            return
            
    end    
    
end

