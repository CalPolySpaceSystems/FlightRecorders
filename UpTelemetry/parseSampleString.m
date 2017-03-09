function [ sampleString ] = parseSampleString(startChar,delimiter,CSDelimiter,endChar,lengthParam)

    if lengthParam
       
        sampleString = strcat([startChar 'LAT' delimiter 'LONG' delimiter 'ALT' delimiter '######' delimiter '#...#' CSDelimiter 'CS' endChar]);
        
    else
        
        sampleString = strcat([startChar 'LAT' delimiter 'LONG' delimiter 'ALT' delimiter '###' delimiter '#...#' CSDelimiter 'LENGTH' delimiter 'CS' endChar]);
        
    end

end

