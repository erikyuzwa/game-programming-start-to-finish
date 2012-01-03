-- This small lua sample demonstrates one sort-a practical way to use
-- scripting. We merely pass in two numbers, our current x position and
-- our elapsed time variable. I think the rest is self-explanatory

-- The only important thing is to return the new xposition

function calculatePosition( arg1, arg2 )

	
        temp = 1.0

	if arg1 > 5.2 then
           arg1 = -5.2
        end

        -- 5.0 is the speed that our object is moving..change at your leisure! :)
	arg1 = arg1 + (temp * 5.0 * arg2)

	

	return arg1

end
