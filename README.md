# Cloudymous
## Project description
A database that uses VRF (Verifiable Random Function) to store cryptographic commitments to sensitive user data (passwords, credit cards/bank account numbers, social security numbers, etc.) without storing any identifiable user information in plain sight (like legal names, usernames, email addresses, etc.). Users will find and retrieve their data using a private key saved on their device, rather than with identifiers saved to the database. That way, if the database is compromised, the malefactor won't have access to sensitive information like they did in the Sony breach or credit bureau breach of last decade. 

## Team
Jordan Jenkins and Ryan Arnesen.

## SQL
MySQL

## No-SQL
Will likely use a key-value NoSQL database like AWS DynamoDB. 

## Business
No current plans to make this a business. I'm expecting this to be more of an open-source project that computer security students/professionals can use for research purposes, security testing, and experimenting with new ideas. 

## Legal
**FOR SECURITY RESEARCH ONLY. DO NOT USE FOR REAL WORLD APPLICATIONS. CONSIDER IT INSECURE.**

## Technical
Our priorities will be:
- Choose client-side programming language
- Choose server-side programming language with robust VRF/cryptographic libraries (C or Go could work)
- Implement the MVP
