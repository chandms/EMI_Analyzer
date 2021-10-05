import { stringify } from '@angular/compiler/src/util';
import { Component, OnInit } from '@angular/core';
import { FormControlName, FormGroup } from '@angular/forms';
import { FormControl } from '@angular/forms';
import { Router } from '@angular/router';
import jwt_decode from "jwt-decode";
import { AuthService } from '../service/auth.service';
import { LoginService } from '../service/login.service';
import { User } from './User';
import { AlertsService } from 'angular-alert-module';


@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css'],
  
})
export class LoginComponent implements OnInit {




  constructor(private service : LoginService, private route : Router, private authService : AuthService, private alerts: AlertsService) {
   }

  ngOnInit(): void {
  }

  username =  new FormControl('');
  password= new FormControl('');
  user: User;
  
 
  

  onSubmit(){
    this.service.decodeUser(this.username.value,this.password.value).subscribe(
      Response => {
        this.user = jwt_decode(Response.token);
        if(this.username.value && this.password.value){
        sessionStorage.setItem('user', JSON.stringify(this.user));
        this.route.navigateByUrl('/proj',{state: jwt_decode(Response.token) });
        }
       
       
      },
      (error: Response) => {
        console.log(error.status);
        console.log(error.statusText)
        this.alerts.setMessage('Error in login','error');
      })
    }
}
