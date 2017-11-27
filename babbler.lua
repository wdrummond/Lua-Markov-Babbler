

function driver ( fname, amount, n ) 

	--get contents of file
	f = creadFile(fname)

	-- print(amount)
	-- print(n)

	--create tokens and insert into table
	tokens = {}
	offset = 0
	token = "token"
	while( offset <= string.len(f) ) do
		token, offset = ctokenizer(f, offset)
		--print(token)
		offset = offset + 1
		table.insert(tokens, token)
	end 

	--get length of table
	local count = 0
	for _,_ in pairs(tokens) do
		count = count + 1
	end

	-- for i = 1, count do
	-- 	print(tokens[i])
	-- 	-- count = count + 1
	-- end

	--make n-grams of size n
	ngrams = {}
	for i,v in pairs(tokens) do
		if count - i == 1 then 
			key = v .. ' ' .. tokens[i + 1]
			value = tokens[1]
		elseif count - i == 0 then
			key = v .. ' ' .. tokens[1]
			value = tokens[2]
		else
			key = tokens[i] .. ' ' .. tokens[i + 1]
			value = tokens[i + 2]
		end
		-- print(key, value)
		ngrams[key] = value
	end

	--test the ngrams
	-- local count1 = 0
	-- for k, e in pairs(ngrams) do
	-- 	print(k, e)
	-- 	count1 = count1 + 1
	-- end
	-- print(count1)


	--babble out the specified amount of words.

	--get a random key
	 randkey = math.randomchoice(ngrams)
	 io.write(ngrams[randkey] .. ' ')


	for j = 1, amount do 
	 		words = {}
			for word in randkey:gmatch("%w+") do 
				table.insert(words, word) 
			end
			randkey = words[2] .. ' ' .. ngrams[randkey]
			io.write(ngrams[randkey] .. ' ')

	end





end

function math.randomchoice(t) --Selects a random item from a table
    local keys = {}
    for key, value in pairs(t) do
    	--print(#keys+1, key)
        keys[#keys+1] = key --Store keys in another table
    end
    index = keys[math.random(1, #keys)]
    --print(index)
    return index
    --return t[index]
end