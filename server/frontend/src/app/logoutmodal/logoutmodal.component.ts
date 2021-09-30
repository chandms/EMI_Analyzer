import { Component, OnInit } from '@angular/core';
import { MatDialogRef } from '@angular/material/dialog';
import { Router } from '@angular/router';

@Component({
  selector: 'app-logoutmodal',
  templateUrl: './logoutmodal.component.html',
  styleUrls: ['./logoutmodal.component.css']
})
export class LogoutmodalComponent implements OnInit {

  constructor(public dialogRef: MatDialogRef<LogoutmodalComponent>, private route : Router) { }

  ngOnInit(): void {
  }
  actionFunction() {
    this.closeModal();
    this.route.navigateByUrl('',{state: null });
  }

  closeModal() {
    this.dialogRef.close();

  }

}
