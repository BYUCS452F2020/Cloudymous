AuthTable(UserName, Password_E, Token)
AuthTable has all of the authentication data, including usernames (which are strings), encrypted passwords(byte array), and authorization tokens (byte array). 
This table is in 4th Normal Form because the non-key values Password_E and Token functionally depend only on the UserName, and there are no mult-value dependencies. 

StoredPassword(PWDID, Password_E, UserName_E)
The StoredPasswords Table contains a unique Id (int), an encrypted version of the password (byte array), and an encrypted version of the UserName, the user name is encrypted differently for each “stored” table so that if someone breaks into the database they can not tell what entries belong to who.(byte array).
This table is in 4th Normal Form because the non-key values Password_E and UserName_E functionally depend only on the PPWID, and there are no mult-value dependencies. 

StoredSSN(SSNID, SSN_E, UserName_E)
The StoredSSN table stores the encrypted versions of user social security numbers (as byte arrays), and encrypted usernames (as byte arrays). The unique id for each row is an int. 
This table is in 4th Normal Form because the non-key values SSN_E and UserName_E functionally depend only on the SSNID, and there are no mult-value dependencies. 

StoredCCN(CCNID, CCN_E, UserName_E)
The StoredCCN table stores the encrypted versions of user credit card numbers (as byte arrays), and encrypted usernames (as byte arrays). The unique id for each row is an int.
This table is in 4th Normal Form because the non-key values CC_E and Username_E functionally depend only on the CCNID, and there are no mult-value dependencies. 

FailedLogs(FailID, UserName, TimeStamp, AttemptCount)
	-Foreign Key UserName references AuthTable
The FailedLogs table keeps track of the number of failed login attempts of users. When the max limit of failed attempts is reached, the user is locked out for a period of time before they can retry. Upon the next login attempt, the timestamp will be checked to see if the user should still be locked out or not. The username is a string, timestamp is a long, and attempt count is an int. The unique id for each row is an int. 
This table is in 4th Normal Form because the non-key values TimeStamp and AttemptCount functionally depend only on the FailID, and there are no mult-value dependencies. 
