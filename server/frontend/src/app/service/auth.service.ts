import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { tap, delay } from 'rxjs/operators';


@Injectable({
  providedIn: 'root'
})
export class AuthService {

  isUserLoggedIn : boolean;
  constructor() { }
  login(userName: string, password: string) {
    console.log(userName);
    console.log(password);
    
    localStorage.setItem('isUserLoggedIn', "true"); 

 return of(this.isUserLoggedIn).pipe(
    delay(1000),
    tap(val => { 
       console.log("Is User Authentication is successful: " + val); 
    })
 );
 }

 logout(): void {
 this.isUserLoggedIn = false;
    localStorage.removeItem('isUserLoggedIn'); 
 }
}
