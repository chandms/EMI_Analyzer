import { Component, OnInit } from '@angular/core';
import { User } from '../login/User';
import { LoginService } from '../service/login.service';
import { MatDialog, MatDialogConfig } from '@angular/material/dialog';
import { LogoutmodalComponent } from '../logoutmodal/logoutmodal.component'; 

@Component({
  selector: 'app-header',
  templateUrl: './header.component.html',
  styleUrls: ['./header.component.css']
})
export class HeaderComponent implements OnInit {

  user: User;
  constructor(private service: LoginService, public matDialog: MatDialog) { }

  ngOnInit(): void {
      this.user = JSON.parse(sessionStorage.getItem('user'));
      if(!this.user){
        this.user = new User();
      }
     // console.log("user "+this.user.firstname+" "+this.user.lastname)
  }

  openModal() {
    const dialogConfig = new MatDialogConfig();
    // The user can't close the dialog by clicking outside its body
    dialogConfig.disableClose = true;
    dialogConfig.id = "app-logoutmodal";
    dialogConfig.height = "250px";
    dialogConfig.width = "400px";
    // https://material.angular.io/components/dialog/overview
    const modalDialog = this.matDialog.open(LogoutmodalComponent, dialogConfig);
  }

}
