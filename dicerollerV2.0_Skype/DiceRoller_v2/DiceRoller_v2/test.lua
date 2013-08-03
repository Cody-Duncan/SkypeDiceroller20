admin = ""

function performRoll (sender, command)
    print(sender)
    print(command)
    return "completed roll successfully"
end

function setAdminName(adminName)
    admin = adminName
    print("set admin name to" .. adminName)
end

function isAdmin(name)
    print(admin == name)
    return (admin == name)
end

result1 = performRoll("string1", "string2")
print(result1)

setAdminName("bb")

result2 = isAdmin("aa")
result3 = isAdmin("bb")

print("result2" .. tostring(result2))
print("result3" .. tostring(result3))