function [settingVal] = configScan(configArray,settingName)
%  configScan Scan array

%  For an n x 2 array, where Column 1 is the variable name 
%  and Column 2 is the value, returns the value that
%  cooresponds to the variable name, or blank if it isnt found

for i = 1 : 127
    
    try
        if strcmp(configArray(i,1),settingName)
        
            settingVal = configArray(i,2);
            return
            
        end
            
    catch % Hit end of array
        settingVal = '';
        
    end

end

end