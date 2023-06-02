# Lock
LCD display for Keypad Lock using Arduino.

Upon initialization, user must set up PIN and password. The number of digits in each can be as long as the user wishes.

After initialization, user may enter password to unlock or reset password.

When entering password, the third wrong password entered will deny user access for 30s. 

To reset password, press * and enter PIN to authorize
- If PIN is correct, the user may reset the password.
- If PIN is incorrect, the user will be redirected back to attempt to enter the correct password.

NOTE: This project does not include the actual locking and unlocking mechanism.
