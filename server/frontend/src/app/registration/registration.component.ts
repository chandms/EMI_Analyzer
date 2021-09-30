import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';
import { Router } from '@angular/router';
import { User } from '../login/User';
import { RegistrationService } from '../service/registration.service';
import { AlertsService } from 'angular-alert-module';
import { NotifierService } from 'angular-notifier';

@Component({
  selector: 'app-registration',
  templateUrl: './registration.component.html',
  styleUrls: ['./registration.component.css']
})
export class RegistrationComponent implements OnInit {
  private readonly notifier: NotifierService;
  username =  new FormControl('');
  password= new FormControl('');
  firstname = new FormControl('');
  lastname = new FormControl('');
  email = new FormControl('');
  user: User;
  constructor(private service : RegistrationService, private route : Router, private alert: AlertsService) { }

  ngOnInit(): void {
  }

  onSubmit(){
    this.service.decodeUser(this.username.value,this.password.value,this.firstname.value,
      this.lastname.value,this.email.value).subscribe(
      Response => {
        this.alert.setMessage('User is Created successfully!!!','success');
        //this.route.navigateByUrl('',{state: null });
       
       
      },
      (error: Response) => {
        this.alert.setMessage('User already Exists!!!','error');
      })


  }

}
