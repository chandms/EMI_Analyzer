import { Component, OnInit } from '@angular/core';
import { User } from '../login/User';
import { LoginService } from '../service/login.service';

@Component({
  selector: 'app-header',
  templateUrl: './header.component.html',
  styleUrls: ['./header.component.css']
})
export class HeaderComponent implements OnInit {

  user: User;
  constructor(private service: LoginService) { }

  ngOnInit(): void {
      this.user = JSON.parse(sessionStorage.getItem('user'));
      console.log("user "+this.user.firstname+" "+this.user.lastname)
  }

}
