function [val] = tryget( targetObj, sourceObj, parameter )

    try
       val = get(targetObj,parameter);
    catch
       val = get(sourceObj,parameter);
    end

end

