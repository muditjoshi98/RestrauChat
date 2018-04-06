# RestraùChat
### Your French Companion for Délicieuse Food

RestraùChat is a chatbot which works on C(UNIX) Socket Programming. A program which can book and cancel tables in a Restaurant.

### How it Works
#### Booking
&nbsp;&nbsp;&nbsp;&nbsp; For Booking The bot ask for:-
1. User's Name
2. Number of Seats
3. Time Slot

Then the bot assign a booking reference number, which is used for cancelling and checking status.

![Image of Booking](/img/book.png)

#### Booking status
&nbsp;&nbsp;&nbsp;&nbsp; For status the bot ask for the reference number given during booking, and show the respective details.

![Image of Status](/img/status.png)

#### Caneclling Booking
&nbsp;&nbsp;&nbsp;&nbsp; For cancellation, the bot will ask for the reference number and check if that number is valid and then show their respective details and ask for confirmation.

![Image of Cancel](/img/cancel.png)

### How to Run
&nbsp;&nbsp;&nbsp;&nbsp; Just compile the `ChefBot_Server.c` and `User_Client.c` files.
</br>

**Starting Server**
>$ gcc ChefBot_Server.c -o ChefBot_Server </br>
$ ./ChefBot_Server

</br>
**Opening Client**
>$ gcc User_Client.c -o User_Client </br>
$ ./User_Client
