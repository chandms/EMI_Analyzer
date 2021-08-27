import { stringify } from '@angular/compiler/src/util';
import { Component, OnInit } from '@angular/core';
import { FormControlName, FormGroup } from '@angular/forms';
import { FormControl } from '@angular/forms';
import { Router } from '@angular/router';
import jwtDecode from 'jwt-decode';
import { LoginService } from '../service/login.service';
import { User } from './User';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {




  constructor(private service : LoginService, private route : Router) {
   }

  ngOnInit(): void {
  }

  username =  new FormControl('');
  password= new FormControl('');
 
  

  onSubmit(){
    this.service.decodeUser(this.username.value,this.password.value).subscribe(
      Response => {
        console.log(jwtDecode(Response.token))
        this.route.navigateByUrl('/map-devices',{state: jwtDecode(Response.token)});
      },
      (error: Response) => {
        console.log(error.status);
        console.log(error.statusText)
        alert("Error in login ")
      })
    }
}
